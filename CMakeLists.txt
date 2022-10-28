cmake_minimum_required(VERSION 3.5)

project(calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
FLEX_TARGET(MyScanner scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

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
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
