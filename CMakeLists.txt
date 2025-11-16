cmake_minimum_required(VERSION 3.10.0)
project(MSCITask VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

include(CTest)
enable_testing()

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")

add_subdirectory(${SRC_DIR})

add_executable(main ${SRC_DIR}/main.cpp)
target_link_libraries(main PRIVATE pricing_model)

add_subdirectory(${TESTS_DIR})
