# Fixed Coupon Bond Pricing
### Requirements
- CMake 3.10 or higher
- C++20 compatible compiler
- Windows 10 and 11, 64-bit
    - Microsoft Visual Studio 2022
- Ubuntu 22.04 and 24.04

### Build
```
mkdir build
cd build
cmake ..
cmake --build .
```
### Run
Provide parameters as command line arguments, if a parameter is missing the program will ask to input
```
cd build
./main --notional 100 --annual_coupon 0.07 --frequency quarterly --discount_rate 0.045 --date 2025-11-15 --maturity 2028.12.1.
```
### Run Tests
```
cd build
ctest --verbose
```
