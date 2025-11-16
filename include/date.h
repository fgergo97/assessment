#pragma once

#include <chrono>
#include <assert.h>
#include <functional>
#include <iostream>

namespace date {

using Date = std::chrono::year_month_day;
using Months = std::chrono::months;
using Days = std::chrono::days;

constexpr int operator-(const Date& lhs, const Date& rhs) {
    using namespace std::chrono;
    assert(lhs >= rhs);
    return (sys_days(lhs) - sys_days(rhs)).count();
}

inline std::optional<Date> parseDate(const std::string strDate) {
    std::istringstream stream(strDate);
    int year, month, day;
    char sep1, sep2;
    stream >> year >> sep1 >> month >> sep2 >> day;
    
    const auto date = Date(std::chrono::year(year), std::chrono::month(month), std::chrono::day(day));
    return date.ok() ? std::optional<Date>(date) : std::nullopt;
}

constexpr bool isValidDate(int y, unsigned m, unsigned d) {
    using namespace std::chrono;
    const year_month_day ymd({year{y}, month{m}, day{d}});
    return ymd.ok();
}

constexpr bool isWeekday(const Date& date) {
    using namespace std::chrono;
    const sys_days days(date);
    const weekday weekDay(days);
    return weekDay.c_encoding() >= 1 && weekDay.c_encoding() <= 5;
}

constexpr bool lastDayOfMonth(const Date& date) {
    using namespace std::chrono;
    const sys_days nextDay(sys_days(date) + days(1));
    return Date(nextDay).day() == day(1);
} 

constexpr Date adjustLastDayOfMonth(const Date& date, const Date& maturity) {
    if (lastDayOfMonth(maturity)) {
        using namespace std::chrono;
        const year_month_day_last lastDay{date.year(), date.month()/last};
        return Date{lastDay};
    }

    return date;
}

constexpr Date adjustWeekend(const Date& date) {
    if (isWeekday(date)) {
        return date;
    }

    using namespace std::chrono;
    const sys_days sysDate(date);
    const weekday weekDay(sysDate);

    const Days fridayDiff(((weekDay - weekday(Friday)).count() + 7) % 7);
    const sys_days prevFriday = sysDate - fridayDiff;
    const Date prevFridayDate(prevFriday);

    if (prevFridayDate.month() == date.month()) {
        return prevFridayDate; 
    } else {
        const Days mondayDiff(((weekday(Monday) - weekDay).count() + 7) % 7);
        const sys_days nextMonday = sysDate + mondayDiff;
        return year_month_day(nextMonday);
    }
}

template <typename... Conditions>
constexpr Date adjustDate(Date date, Conditions&&... conditions) {
    ((date = std::invoke(std::forward<Conditions>(conditions), date)), ...);
    return date;
}

} // namespace date