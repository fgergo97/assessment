#include <iostream>

#include "fixedcouponbond.h"
#include "inputparser.h"

int main(int argc, char** argv){
    std::cout << "Fixed Coupon Bond Pricing" << std::endl;
    input_parser::InputParams params(argc, argv);
    auto pricingModel = pricing_model::FixedCouponBond(params);
    const double price = pricingModel.getPrice();
    std::cout << std::setprecision(10) << "Bond Price: " << price << std::endl;

    return 0;
}
