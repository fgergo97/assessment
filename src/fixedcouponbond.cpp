#include "fixedcouponbond.h"

#include <cmath>
#include <numeric>

namespace pricing_model {

using date::operator-;

constexpr float DAYS_IN_A_YEAR = 365; // 260 in case weekends shouldn't be counted

FixedCouponBond::FixedCouponBond(const input_parser::InputParams& params)
    : notional(params.getParam<typeof(notional)>("notional"))
    , annualCoupon(params.getParam<typeof(annualCoupon)>("annual_coupon"))
    , frequency(params.getParam<typeof(frequency)>("frequency"))
    , discountRate(params.getParam<typeof(discountRate)>("discount_rate"))
    , currentDate(params.getParam<typeof(currentDate)>("date"))
    , maturity(params.getParam<typeof(maturity)>("maturity"))
    , daysUntilMaturity(maturity - currentDate), paymentStep(12/frequency)
{
    assert(notional > 0);
    assert(date::isWeekday(maturity));
    assert(maturity >= currentDate);
}

FixedCouponBond::FixedCouponBond(float notional, float annualCoupon, Frequency frequency, 
                                 float discountRate, const Date& date, const Date& maturity)
    : notional(notional), annualCoupon(annualCoupon), frequency(frequency)
    , discountRate(discountRate), currentDate(date), maturity(maturity)
    , daysUntilMaturity(maturity - currentDate), paymentStep(12/frequency)
{
    assert(notional > 0);
    assert(date::isWeekday(maturity));
    assert(maturity >= currentDate);
}

double FixedCouponBond::getPrice() const {
    return faceValue() + couponPresentValue();
}

double FixedCouponBond::faceValue() const {
    return notional * std::exp(-discountRate * (daysUntilMaturity/DAYS_IN_A_YEAR));
}

double FixedCouponBond::couponPresentValue() const {
    const double coupon = couponAmount();

    auto payments = getPaymentDates();
    const double discountedCoupons = std::accumulate(std::cbegin(payments), std::cend(payments), 
        0.0, [ &discountRate = discountRate, &date = currentDate ](double sum, const auto& paymentDate) {
            return sum + std::exp(-discountRate * ((paymentDate - date)/DAYS_IN_A_YEAR));
    });

    return coupon * discountedCoupons;
}

double FixedCouponBond::couponAmount() const {
    return (annualCoupon*notional) / static_cast<int>(frequency);
}

int FixedCouponBond::getPaymentsCount() const {
    return (daysUntilMaturity/DAYS_IN_A_YEAR) * static_cast<int>(frequency);
}

std::vector<FixedCouponBond::Date> FixedCouponBond::getPaymentDates() const {
    const int payments = getPaymentsCount();
    std::vector<Date> dates;

    for (int i = 0; i < payments; ++i) {
        const Date paymentDate = maturity - i*paymentStep;
        // Ordering of conditions is important, first adjust payment date based
        // on maturity date then check if it falls on weekend and adjust if needed
        // std::chrono makes sure that date cannot be invalid
        dates.push_back(date::adjustDate(paymentDate,
            [ &maturity = maturity ] (const Date& date) { return date::adjustLastDayOfMonth(date, maturity); },
            [] (const Date& date) { return date::adjustWeekend(date); }
        ));
    }

    std::for_each(std::cbegin(dates), std::cend(dates), [](const auto& date) {
        std::cout << "  Payment Date: " << date << std::endl;
    });

    return dates;
}

} // namespace pricing_model