cmake_minimum_required(VERSION 3.20)

project(Approximation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Bezier_Curve
        main.cpp
        Approximation.cpp
)