#include "utils.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <random>

void initializeCarrierShippingCosts(std::vector<SKU> &skuList)
{
    for (auto &sku : skuList)
    {
        bool randBool = (getRandomInt(0, 1) == 0);
        std::cout << randBool;
        if (sku.shippingCarrier == "Carrier A")
        {
            sku.carrierAShippingCost = sku.shippingCost;
            sku.carrierBShippingCost = round2(sku.shippingCost + (randBool ? -0.5 * sku.shippingCost : 0.5 * sku.shippingCost));
            sku.carrierCShippingCost = round2(sku.shippingCost + (randBool ? 0.5 * sku.shippingCost : -0.5 * sku.shippingCost));
        }
        else if (sku.shippingCarrier == "Carrier B")
        {
            sku.carrierBShippingCost = sku.shippingCost;
            sku.carrierAShippingCost = round2(sku.shippingCost + (randBool ? -0.5 * sku.shippingCost : 0.5 * sku.shippingCost));
            sku.carrierCShippingCost = round2(sku.shippingCost + (randBool ? 0.5 * sku.shippingCost : -0.5 * sku.shippingCost));
        }
        else if (sku.shippingCarrier == "Carrier C")
        {
            sku.carrierCShippingCost = sku.shippingCost;
            sku.carrierAShippingCost = round2(sku.shippingCost + (randBool ? -0.5 * sku.shippingCost : 0.5 * sku.shippingCost));
            sku.carrierBShippingCost = round2(sku.shippingCost + (randBool ? 0.5 * sku.shippingCost : -0.5 * sku.shippingCost));
        }
        else
        {
            // Default behavior if carrier unknown
            sku.carrierAShippingCost = sku.shippingCost;
            sku.carrierBShippingCost = sku.shippingCost + (randBool ? -0.5 * sku.shippingCost : 0.5 * sku.shippingCost);
            sku.carrierCShippingCost = sku.shippingCost + (!randBool ? -0.5 * sku.shippingCost : 0.5 * sku.shippingCost);
        }
    }
}
void initializeHoldingCostAndOrderingCost(std::vector<SKU> &skuList)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> holdingCostDist(0.20, 0.30);  // 20% to 30%
    std::uniform_real_distribution<> orderingCostDist(30.0, 50.0); // $30 to $50

    for (auto &sku : skuList)
    {
        sku.holdingCostRatio = holdingCostDist(gen);
        sku.orderingCost = orderingCostDist(gen);
    }
}
std::vector<SKU> readSkuList(std::ifstream &file, bool containsSupplierLeadTime)
{
    if (!file.is_open())
        throw std::runtime_error("Could not open the file!");

    std::vector<SKU> res{};
    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        SKU skuObj;
        std::string token;

        std::getline(ss, skuObj.productType, ',');
        std::getline(ss, skuObj.sku, ',');
        std::getline(ss, token, ',');
        skuObj.price = std::stod(token);
        std::getline(ss, token, ',');
        skuObj.availability = std::stoi(token);
        std::getline(ss, token, ',');
        skuObj.numberSold = std::stoi(token);
        std::getline(ss, token, ',');
        skuObj.revenue = std::stod(token);
        std::getline(ss, skuObj.customerDemographics, ',');
        std::getline(ss, token, ',');
        skuObj.stockLevels = std::stoi(token);
        std::getline(ss, token, ',');
        skuObj.leadTimes = std::stoi(token);
        std::getline(ss, token, ',');
        skuObj.orderQuantities = std::stoi(token);
        std::getline(ss, token, ',');
        skuObj.shippingTimes = std::stoi(token);
        std::getline(ss, skuObj.shippingCarrier, ',');
        std::getline(ss, token, ',');
        skuObj.shippingCost = std::stod(token);
        std::getline(ss, skuObj.supplierName, ',');
        std::getline(ss, skuObj.supplierLocation, ',');
        if (containsSupplierLeadTime)
            std::getline(ss, token, ',');
        // skuObj.supplierLeadTime = std::stod(token);
        std::getline(ss, token, ',');
        skuObj.productionVolumes = std::stoi(token);
        std::getline(ss, token, ',');
        skuObj.manufacturingLeadTime = std::stod(token);
        std::getline(ss, token, ',');
        skuObj.manufacturingCost = std::stod(token);
        std::getline(ss, skuObj.inspectionResults, ',');
        std::getline(ss, token, ',');
        skuObj.defectRates = std::stod(token);
        std::getline(ss, skuObj.transportationMode, ',');
        std::getline(ss, skuObj.route, ',');
        res.push_back(skuObj);
    }

    file.close();
    initializeCarrierShippingCosts(res);
    initializeHoldingCostAndOrderingCost(res);
    return res;
}

void printSkuList(const std::vector<SKU> &skuList)
{
    for (auto &sku : skuList)
    {
        std::cout << sku.toString() << std::endl;
    }
}

double round2(double value)
{
    return std::round(value * 100.0) / 100.0;
}

double getRandomDouble(double min, double max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}
int getRandomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void cleanData(std::vector<SKU> &data)
{
    for (auto &sku : data)
    {
        // Round monetary fields
        sku.price = round2(sku.price);
        sku.revenue = round2(sku.revenue);
        sku.shippingCost = round2(sku.shippingCost);
        sku.manufacturingCost = round2(sku.manufacturingCost);

        // Enforce pricing rules
        if (sku.price < sku.manufacturingCost + sku.shippingCost || sku.shippingCost > 0.25 * sku.price)
        {
            double markup = getRandomDouble(1.0, 3.0);
            sku.price = round2((sku.manufacturingCost + sku.shippingCost) * (1.0 + markup));
        }

        // Recalculate revenue
        sku.revenue = round2(sku.price * sku.numberSold);

        // Cap defect rates at 1%
        if (sku.defectRates > 0.01)
        {
            sku.defectRates = 0.01;
        }

        // Inspection result based on defect rate > 2%
        if (sku.defectRates > 0.02)
        {
            sku.inspectionResults = "Fail";
        }

        // Lead time adjustments based on transportation mode
        if (sku.transportationMode == "Air" && sku.leadTimes > 7)
        {
            sku.leadTimes = getRandomInt(2, 7);
        }
        else if (sku.transportationMode == "Sea" && sku.leadTimes < 15)
        {
            sku.leadTimes = getRandomInt(15, 30);
        }

        // Manufacturing lead time between 3–15 days
        if (sku.manufacturingLeadTime < 3 || sku.manufacturingLeadTime > 15)
        {
            sku.manufacturingLeadTime = getRandomInt(3, 15);
        }
    }
}

void writeCleanedCSV(const std::vector<SKU> &data, const std::string &filename)
{
    try
    {
        std::ofstream file(filename);
        file << "Product type,SKU,Price,Availability,Number of products sold,Revenue generated,Customer demographics,"
                "Stock levels,Lead times,Order quantities,Shipping times,Shipping carriers,Shipping costs,Supplier name,"
                "Location,Production volumes,Manufacturing lead time,Manufacturing costs,Inspection results,Defect rates,"
                "Transportation modes,Routes\n";

        for (const auto &sku : data)
        {
            file << sku.commaSeparatedToString() << "\n";
        }

        file.close();
        std::cout << "Cleaned data saved to " << filename << std::endl;
    }
    catch (...)
    {
        throw std::runtime_error("Could not save the cleaned file!");
    }
}