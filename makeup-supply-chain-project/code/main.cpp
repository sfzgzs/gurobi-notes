#include "gurobi_c++.h"
#include "SKU.h"
#include "utils.h"
#include <iostream>
#include <fstream>

int main()
{
     try
     {
          std::ifstream file("supply_chain_data_makeup_store.csv");
          std::vector<SKU> skuList = readSkuList(file);
          cleanData(skuList);
          writeCleanedCSV(skuList, "cleaned_supply_chain_data.csv");

          GRBEnv env = GRBEnv(true);
          env.set("LogFile", "test.log");
          env.start();

          GRBModel model = GRBModel(env);
          // TODO Optimize
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

     return 0;
}
