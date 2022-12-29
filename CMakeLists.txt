cmake_minimum_required(VERSION 3.5)

project(calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)
add_executable(calculator
    main.cpp
    expression.cpp
    value.cpp
    verylong.cpp
    identity.cpp
)
