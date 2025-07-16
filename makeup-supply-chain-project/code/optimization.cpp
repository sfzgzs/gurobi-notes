#include "gurobi_c++.h"
#include "SKU.h"
#include "utils.h"
#include "optimization.h"
// #include <iostream>
// #include <fstream>
#include <iomanip>

void solveProblem1_CarrierAssignment(std::vector<SKU> &skuList)
{
    try
    {
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "carrier_assignment.log");
        env.start();

        GRBModel model = GRBModel(env);

        int n = skuList.size();

        // Decision variables: x_iA, x_iB, x_iC: How many packages in SKU_i are shipped using each carrier
        std::vector<GRBVar> xA(n), xB(n), xC(n);
        for (int i = 0; i < n; ++i)
        {
            xA[i] = model.addVar(0, GRB_INFINITY, 0, GRB_INTEGER, "xA_" + std::to_string(i));
            xB[i] = model.addVar(0, GRB_INFINITY, 0, GRB_INTEGER, "xB_" + std::to_string(i));
            xC[i] = model.addVar(0, GRB_INFINITY, 0, GRB_INTEGER, "xC_" + std::to_string(i));
        }

        // Constraint: we have at least ordered enough that (considering the current stock) would fulfill what customers have bought so far
        for (int i = 0; i < n; ++i)
        {
            model.addConstr((xA[i] + xB[i] + xC[i]) * skuList[i].orderQuantities >= skuList[i].numberSold - skuList[i].stockLevels, "fufill_the_order_of_sku_" + skuList[i].sku);
        }

        // Variables for total packages per carrier
        GRBVar packagesByA = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "packagesByA");
        GRBVar packagesByB = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "packagesByB");
        GRBVar packagesByC = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "packagesByC");

        // Calculate total packages per carrier (linear constraints)
        GRBLinExpr sumOfPackagesByA = 0, sumOfPackagesByB = 0, sumOfPackagesByC = 0;
        for (int i = 0; i < n; ++i)
        {
            sumOfPackagesByA += xA[i];
            sumOfPackagesByB += xB[i];
            sumOfPackagesByC += xC[i];
        }
        model.addConstr(packagesByA == sumOfPackagesByA, "packagesByA_def");
        model.addConstr(packagesByB == sumOfPackagesByB, "packagesByB_def");
        model.addConstr(packagesByC == sumOfPackagesByC, "packagesByC_def");

        // Discount binaries
        GRBVar discountByA = model.addVar(0, 1, 0, GRB_BINARY, "discountByA");
        GRBVar discountByB = model.addVar(0, 1, 0, GRB_BINARY, "discountByB");
        GRBVar discountByC = model.addVar(0, 1, 0, GRB_BINARY, "discountByC");

        model.addGenConstrIndicator(discountByA, 1, packagesByA >= 2500, "discountByA_active");
        model.addGenConstrIndicator(discountByA, 0, packagesByA <= 2499, "discountByA_inactive");

        model.addGenConstrIndicator(discountByB, 1, packagesByB >= 2600, "discountByB_active");
        model.addGenConstrIndicator(discountByB, 0, packagesByB <= 2599, "discountByB_inactive");

        model.addGenConstrIndicator(discountByC, 1, packagesByC >= 2400, "discountByC_active");
        model.addGenConstrIndicator(discountByC, 0, packagesByC <= 2399, "discountByC_inactive");

        // model.addConstr(packagesByA >= 10, "min_use_A");
        // model.addConstr(packagesByB >= 20, "min_use_B");
        // model.addConstr(packagesByC >= 10, "min_use_C");

        // Calculate shipping cost per carrier
        GRBLinExpr costA = 0, costB = 0, costC = 0;
        GRBLinExpr allProductsManufacturingCosts = 0;
        for (int i = 0; i < n; ++i)
        {
            costA += skuList[i].carrierAShippingCost * xA[i];
            costB += skuList[i].carrierBShippingCost * xB[i];
            costC += skuList[i].carrierCShippingCost * xC[i];
            allProductsManufacturingCosts += skuList[i].manufacturingCost * (xA[i] + xB[i] + xC[i]) * skuList[i].orderQuantities;
        }

        // Objective: minimize total cost after discount
        GRBQuadExpr totalShippingCost = costA * (1 - 0.25 * discountByA) + costB * (1 - 0.30 * discountByB) + costC * (1 - 0.20 * discountByC);

        model.setObjective(totalShippingCost + allProductsManufacturingCosts, GRB_MINIMIZE);

        // Optimize
        model.optimize();
        double totalRevenue = 0;
        // After optimization, assign carriers and print result
        for (int i = 0; i < n; ++i)
        {
            totalRevenue += skuList[i].price * (xA[i].get(GRB_DoubleAttr_X) + xB[i].get(GRB_DoubleAttr_X) + xC[i].get(GRB_DoubleAttr_X)) * skuList[i].orderQuantities;
            // if (xA[i].get(GRB_DoubleAttr_X) != 0)
            std::cout << "SKU " << i << ", Carrier A:"
                      << " with cost " << skuList[i].carrierAShippingCost
                      << "ships " << xA[i].get(GRB_DoubleAttr_X) << "\n";
            // if (xB[i].get(GRB_DoubleAttr_X) != 0)
            std::cout << "SKU " << i << ", Carrier B:"
                      << " with cost " << skuList[i].carrierBShippingCost
                      << "ships " << xB[i].get(GRB_DoubleAttr_X) << "\n";
            // if (xC[i].get(GRB_DoubleAttr_X) != 0)
            std::cout << "SKU " << i << ", Carrier C:"
                      << " with cost " << skuList[i].carrierCShippingCost
                      << "ships " << xC[i].get(GRB_DoubleAttr_X) << "\n";
            std::cout << "\n ---- \n";
        }
        std::cout << std::fixed << std::fixed << std::setprecision(2) << "Total cost of shipping and manufacturing: " << model.get(GRB_DoubleAttr_ObjVal) << ", Total Revenue:" << totalRevenue << " Total Profit: " << (totalRevenue - model.get(GRB_DoubleAttr_ObjVal)) << std::endl;
    }
    catch (GRBException e)
    {
        std::cout << "Error code = " << e.getErrorCode() << std::endl;
        std::cout << e.getMessage() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception during optimization" << std::endl;
    }
}
