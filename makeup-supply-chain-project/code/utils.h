#pragma once
#include "SKU.h"
#include <vector>
#include <fstream>

std::vector<SKU> readSkuList(std::ifstream &file);
void printSkuList(const std::vector<SKU> &skuList);
void cleanData(std::vector<SKU> &data);
void writeCleanedCSV(const std::vector<SKU> &data, const std::string &filename);