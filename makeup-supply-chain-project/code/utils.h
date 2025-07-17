#pragma once
#include "SKU.h"
#include <vector>
#include <fstream>

const int ORDERING_COST_FOR_MORE_THAN_ONE_SHIPPING = 10;
void initializeCarrierShippingCosts(std::vector<SKU> &skuList);
void initializeHoldingCostAndOrderingCost(std::vector<SKU> &skuList);
double round2(double value);
int getRandomInt(int min, int max);
std::vector<SKU> readSkuList(std::ifstream &file, bool containsSupplierLeadTime);
void printSkuList(const std::vector<SKU> &skuList);
void cleanData(std::vector<SKU> &data);
void writeCleanedCSV(const std::vector<SKU> &data, const std::string &filename);