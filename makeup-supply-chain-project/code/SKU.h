#pragma once
#include <string>
#include <vector>

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
    // When shipping, how many products of this fits in one shipping package
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

    // Modification for shipping problem
    double carrierAShippingCost;
    double carrierBShippingCost;
    double carrierCShippingCost;

    std::string toString() const;
    std::string commaSeparatedToString() const;
};