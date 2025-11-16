add_library(pricing_model fixedcouponbond.cpp inputparser.cpp)

target_include_directories(pricing_model
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)