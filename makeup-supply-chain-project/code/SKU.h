#pragma once
#include <string>

struct SKU
{
    std::string productType;
    std::string sku;
    double price;
    double availability;
    int numberSold;
    double revenue;
    std::string customerDemographics;
    int stockLevels;
    double leadTimes;
    int orderQuantities;
    double shippingTimes;
    std::string shippingCarrier;
    double shippingCost;
    std::string supplierName;
    std::string supplierLocation;
    // double supplierLeadTime; // Kept only one lead time
    int productionVolumes;
    double manufacturingLeadTime;
    double manufacturingCost;
    std::string inspectionResults;
    double defectRates;
    std::string transportationMode;
    std::string route;
    std::string toString() const;
    std::string commaSeparatedToString() const;
};
