cmake_minimum_required(VERSION 3.17)
set (CMAKE_CXX_STANDARD 20)

project(BigInteger)

add_executable(
        BigInteger
        src/BigInteger.cpp
        src/BigInteger.hpp)