cmake_minimum_required(VERSION 3.17)
PROJECT(BigInteger)
set(CMAKE_CXX_FLAGS "-std=c++20")


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(
        BigInteger
        src/BigInteger.cpp)
message(${CONAN_LIBS})
target_link_libraries(BigInteger ${CONAN_LIBS})