#pragma once

#include <cstdint>

#include "date.h"
#include "frequency.h"
#include "inputparser.h"

namespace pricing_model {

class FixedCouponBond {
    using Date = date::Date;
    using Months = date::Months;

public:
    FixedCouponBond(const input_parser::InputParams& params);
    FixedCouponBond(float notional, float annualCoupon, Frequency frequency,
                    float discountRate, const Date& maturity, const Date& date);

    double getPrice() const;

private:
    double faceValue() const;
    double couponPresentValue() const;
    double couponAmount() const;
    int getPaymentsCount() const;
    std::vector<Date> getPaymentDates() const;

    float notional = 0;
    float annualCoupon = 0;
    Frequency frequency = Frequency::ANNUAL;
    float discountRate = 0;
    Date currentDate;
    Date maturity;
    int daysUntilMaturity = 0;
    Months paymentStep;
};

} // namespace pricing_model