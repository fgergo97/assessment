add_executable(tests test_pricingmodel.cpp)

target_link_libraries(tests PRIVATE pricing_model)

add_test(NAME PricingModelTests COMMAND tests)