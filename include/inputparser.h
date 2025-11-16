#pragma once 

#include <iostream>
#include <cstring>
#include <cctype>
#include <functional>

#include "frequency.h"
#include "date.h"

namespace input_parser {

template <typename T>
struct stringConverter {
    static T fromString(const std::string& str) {
        static_assert(sizeof(T) == 0, "No string converter defined for the type");
    }
};

template <>
struct stringConverter<float> {
    static float fromString(const std::string& str) {
        return std::stof(str);
    }
};

template <>
struct stringConverter<Frequency> {
    static std::optional<Frequency> fromString(const std::string& str) {
        std::string copy = str;
        std::transform(std::begin(copy), std::end(copy), std::begin(copy), ::tolower);
        const auto it = strToFrequency.find(copy);
        if (it != std::cend(strToFrequency)) {
            return it->second;
        } else {
            return {};
        }
    }
};

template <>
struct stringConverter<date::Date> {
    static std::optional<date::Date> fromString(const std::string& str) {
        return date::parseDate(str);
    }
};

class IInputParam {
public:
    virtual ~IInputParam() = default;

    virtual std::string getName() const = 0; 
    virtual std::string getTooltip() const = 0; 
    virtual void setValue(const std::string_view& value) = 0;
    virtual bool hasValue() const = 0;
    virtual void input() = 0;
};

template <class T>
class InputParam : public IInputParam {
public:
    InputParam(const std::string& name, const std::string& tooltip, const std::function<bool(const T&)>& validator = nullptr) 
        : name(name), tooltip(tooltip), validator(validator)
    {}

    std::string getName() const override {
        return name;
    }
    std::string getTooltip() const override {
        return tooltip;
    }
    void setValue(const std::string_view& value) override {
        param = stringConverter<T>::fromString(std::string(value));

        if (validator && param.has_value()) {
            if (!validator(*param)) {
                param = std::nullopt;
            }
        }

        if (param.has_value()) {
            std::cout << "Set parameter " << name << " to " << *param << std::endl;
        } else {
            std::cerr << "Failed to parse parameter " + name + " with value " + std::string(value) << std::endl;
        }
    }
    bool hasValue() const override {
        return param.has_value();
    }
    void input() override {
        do {
            std::cout << "Enter value for " << name << " (" << getTooltip() <<"): ";
            std::string consoleInput;
            std::cin >> consoleInput;
            setValue(consoleInput);
        } while (!hasValue());
    }
    T getValue() const {
        return *param;
    }

private:
    std::string name;
    std::string tooltip;
    std::function<bool(const T&)> validator;

    std::optional<T> param;
};

class InputParams {
public:
    InputParams(int argc, char** argv)  {
        parse(argc, argv);
        checkParams();
    }

    template <class T>
    T getParam(const std::string& name) const {
        for (const auto& param : params) {
            if (param->getName() == name) {
                return static_cast<InputParam<T>*>(param.get())->getValue();
            }
        }
        throw std::runtime_error("Parameter " + name + " not found!");
    }

    bool isParamSet(const std::string& name) const {
        for (const auto& param : params) {
            if (param->getName() == name) {
                return param->hasValue();
            }
        }
        throw std::runtime_error("Parameter " + name + " not found!");
    }


private:
    void parse(int argc, char** argv);
    void input(const char* inputParam, const char* inputArgument);
    void checkParams();

    std::vector<std::shared_ptr<IInputParam>> params = {
        std::make_shared<InputParam<float>>("notional", "E.g., 100"),
        std::make_shared<InputParam<float>>("annual_coupon", "Input as fractional, e.g., 5% as 0.05"),
        std::make_shared<InputParam<Frequency>>("frequency", "Options: annual, quarterly, monthly"),
        std::make_shared<InputParam<date::Date>>("date", "Format: YYYY-MM-DD",
            [this] (const date::Date& date) mutable {
                if (isParamSet("maturity") && date >= getParam<date::Date>("maturity")) {
                    std::cerr << "Current date must be before maturity date!" << std::endl;
                    return false;
                }
                return true;
            }),
        std::make_shared<InputParam<date::Date>>("maturity", "Format: YYYY-MM-DD", 
            [this] (const date::Date& maturity) mutable {
                if (!date::isWeekday(maturity)) {
                    std::cerr << "Maturity date must be a weekday!" << std::endl;
                    return false;
                }
                if (isParamSet("date") && maturity <= getParam<date::Date>("date")) {
                    std::cerr << "Maturity date must be after current date!" << std::endl;
                    return false;
                }
                return true;
            }),
        std::make_shared<InputParam<float>>("discount_rate", "Input as fractional, e.g., 3% as 0.03"),
    };
};

} // namespace input_parser