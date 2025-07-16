#include "SKU.h"
#include <sstream>

std::string SKU::toString() const
{
    return "product Type: " + productType +
           "| sku: " + sku +
           "| price: " + std::to_string(price) +
           "| availability: " + std::to_string(availability) +
           "| number Sold: " + std::to_string(numberSold) +
           "| revenue: " + std::to_string(revenue) +
           "| customer Demographics: " + customerDemographics +
           "| stock Levels: " + std::to_string(stockLevels) +
           "| lead Times: " + std::to_string(leadTimes) +
           "| order Quantities: " + std::to_string(orderQuantities) +
           "| shipping Times: " + std::to_string(shippingTimes) +
           "| shipping Carrier: " + shippingCarrier +
           "| shipping Cost: " + std::to_string(shippingCost) +
           "| supplier Name: " + supplierName +
           "| supplier Location: " + supplierLocation +
           //    "| supplier Lead Time: " + std::to_string(supplierLeadTime) +
           "| production Volumes: " + std::to_string(productionVolumes) +
           "| manufacturing Lead Time: " + std::to_string(manufacturingLeadTime) +
           "| manufacturing Cost: " + std::to_string(manufacturingCost) +
           "| inspection Results: " + inspectionResults +
           "| defect Rates: " + std::to_string(defectRates) +
           "| transportation Mode: " + transportationMode +
           "| route: " + route;
}

std::string SKU::commaSeparatedToString() const
{
    return productType +
           "," + sku +
           "," + std::to_string(price) +
           "," + std::to_string(availability) +
           "," + std::to_string(numberSold) +
           "," + std::to_string(revenue) +
           "," + customerDemographics +
           "," + std::to_string(stockLevels) +
           "," + std::to_string(leadTimes) +
           "," + std::to_string(orderQuantities) +
           "," + std::to_string(shippingTimes) +
           "," + shippingCarrier +
           "," + std::to_string(shippingCost) +
           "," + supplierName +
           "," + supplierLocation +
           "," + std::to_string(productionVolumes) +
           "," + std::to_string(manufacturingLeadTime) +
           "," + std::to_string(manufacturingCost) +
           "," + inspectionResults +
           "," + std::to_string(defectRates) +
           "," + transportationMode +
           "," + route;
}
