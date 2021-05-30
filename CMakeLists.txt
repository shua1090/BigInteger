cmake_minimum_required(VERSION 3.17)
PROJECT(BigInteger)

add_executable(
        BigInteger
        src/BigInteger.cpp src/Addition.cpp src/Subtraction.cpp src/Transformation.cpp src/Comparison.cpp src/Multiplication.cpp src/Division.cpp)
