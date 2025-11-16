#include "inputparser.h"

namespace input_parser {

constexpr const char* ARG_PREFIX = "--";

void InputParams::parse(int argc, char** argv) {
    for (int i = 1; i < argc; i+=2) {
        if (argv[i+1]) {
            input(argv[i], argv[i+1]);
        }
    }
}

void InputParams::input(const char* inputParam, const char* inputArgument) {
    std::string_view arg = inputParam;
    if (arg.starts_with(ARG_PREFIX)) {
        arg.remove_prefix(std::strlen(ARG_PREFIX));
    }

    auto it = std::find_if(std::begin(params), std::end(params), [ arg = &arg ](const auto& param) {
        return *arg == param->getName();
    });
    if (it != std::cend(params)) {
        (*it)->setValue(inputArgument);
    } else {
        std::cerr << "Unknown parameter: " << arg << std::endl;
    }
}

void InputParams::checkParams() {
    std::for_each(std::begin(params), std::end(params), [](const auto& param) {
        if (!param->hasValue()) {
            param->input();
        }
    });
}

} // namespace input_parser