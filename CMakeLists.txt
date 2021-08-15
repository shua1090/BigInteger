cmake_minimum_required(VERSION 3.17)
project(BigInteger)

set_property(GLOBAL PROPERTY CXX_STANDARD 20)

add_executable(
        BigInteger
        src/BigInteger.cpp)

target_link_libraries(BigInteger ${CONAN_LIBS})
