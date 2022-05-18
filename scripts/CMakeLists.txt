cmake_minimum_required(VERSION 3.21)
project(PrawnstarScripts)
set(CMAKE_CXX_STANDARD 17)

set (OPENSSL_ROOT_DIR /usr/local/opt/openssl@3)
find_package(OpenSSL REQUIRED)

include_directories(/usr/local/include
        /usr/local/opt/openssl@3/include
        ../include)
add_executable(eccGenerateSignVerifyHash eccGenerateSignVerifyHash.cpp)
target_link_libraries(eccGenerateSignVerifyHash OpenSSL::Crypto)

add_executable(eccGenKeyToDisk eccGenKeyToDisk.cpp)
target_link_libraries(eccGenKeyToDisk OpenSSL::Crypto)

add_executable(jsonSerialization jsonSerialization.cpp)

add_executable(jsontest jsontest.cpp)
add_executable(liquidity-test liquidity-test.cpp)
add_executable(maptest maptest.cpp)

add_executable(sundeep_test sundeep_test.cpp)
target_link_libraries(sundeep_test OpenSSL::Crypto)
add_executable(template template.cpp)
add_executable(vector vector.cpp)
add_executable(vectormap vectormap.cpp)

