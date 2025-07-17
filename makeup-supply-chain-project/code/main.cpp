#include <iostream>
#include <fstream>
#include <iomanip>

#include "gurobi_c++.h"
#include "SKU.h"
#include "utils.h"
#include "optimization.h"
int main()
{
     try
     {
          // The following is the logic for cleaning up the database:
          /*
          std::ifstream file("supply_chain_data_makeup_store.csv");
          std::vector<SKU> skuList = readSkuList(file, true);
          cleanData(skuList);
          writeCleanedCSV(skuList, "cleaned_supply_chain_data.csv");
          */
          std::ifstream file("cleaned_supply_chain_data.csv");
          std::vector<SKU> skuList = readSkuList(file, false);
          solveProblem1_CarrierAssignment(skuList);
          solveProblem2_CarrierAssignment(skuList);
     }
     catch (GRBException e)
     {
          std::cout << "Error code = " << e.getErrorCode() << std::endl;
          std::cout << e.getMessage() << std::endl;
     }
     catch (const std::exception &e)
     {
          std::cout << "Error code = " << e.what() << std::endl;
     }
     catch (...)
     {
          std::cout << "Exception during optimization" << std::endl;
     }

     std::cout << "HAHA FINISHED!";
     return 0;
}