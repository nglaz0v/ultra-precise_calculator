cmake_minimum_required(VERSION 3.5)

project(calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(calculator
    calculator.cpp
    expression.cpp
    expression.h
    identity.cpp
    identity.h
    value.cpp
    value.h
    verylong.cpp
    verylong.h
)
