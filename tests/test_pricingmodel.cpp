#include <assert.h>

#include "fixedcouponbond.h"

void testPricingAnnual() {
    pricing_model::FixedCouponBond bond(
        100,        // notional
        0.05,         // annual coupon
        Frequency::ANNUAL, // frequency
        0.03,         // discount rate
        date::Date{std::chrono::year{2025}, std::chrono::month{11}, std::chrono::day{15}},  // current date
        date::Date{std::chrono::year{2028}, std::chrono::month{11}, std::chrono::day{15}} // maturity
    );
    assert(std::abs(bond.getPrice() - 105.517) < 1e-3) ;
}

void testPricingQuarterly() {
    pricing_model::FixedCouponBond bond(
        100,        // notional
        0.07,         // annual coupon
        Frequency::QUARTERLY, // frequency
        0.045,         // discount rate
        date::Date{std::chrono::year{2023}, std::chrono::month{5}, std::chrono::day{1}},  // current date
        date::Date{std::chrono::year{2026}, std::chrono::month{5}, std::chrono::day{1}} // maturity
    );
    assert(std::abs(bond.getPrice() - 106.889) < 1e-3) ;
}

void testPricingMonthly() {
    pricing_model::FixedCouponBond bond(
        1000,        // notional
        0.05,         // annual coupon
        Frequency::MONTHLY, // frequency
        0.03,         // discount rate
        date::Date{std::chrono::year{2022}, std::chrono::month{11}, std::chrono::day{8}},  // current date
        date::Date{std::chrono::year{2025}, std::chrono::month{10}, std::chrono::day{1}} // maturity
    );
    assert(std::abs(bond.getPrice() - 1052.093) < 1e-3) ;
}

void testPricingPaymentFallsOnDate() {
    pricing_model::FixedCouponBond bond(
        1000,        // notional
        0.05,         // annual coupon
        Frequency::MONTHLY, // frequency
        0.03,         // discount rate
        date::Date{std::chrono::year{2025}, std::chrono::month{10}, std::chrono::day{3}},  // current date
        date::Date{std::chrono::year{2025}, std::chrono::month{11}, std::chrono::day{3}} // maturity
    );
    assert(std::abs(bond.getPrice() - 1001.611) < 1e-3) ;
}

int main() {
    testPricingAnnual();
    testPricingQuarterly();
    testPricingMonthly();
    testPricingPaymentFallsOnDate();
    return 0;
}