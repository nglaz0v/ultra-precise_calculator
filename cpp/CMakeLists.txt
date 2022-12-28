cmake_minimum_required(VERSION 3.5)

project(calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser syntax.y ${CMAKE_CURRENT_BINARY_DIR}/syntax.cc)
FLEX_TARGET(MyScanner lexicon.l ${CMAKE_CURRENT_BINARY_DIR}/lexicon.cc)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(calculator
    # identity.cpp
    # identity.h
    # verylong.cpp
    # verylong.h
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)