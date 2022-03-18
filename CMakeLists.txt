cmake_minimum_required(VERSION 3.0.0)

project(PrawnstarBlockchain VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)

add_library(PrawnstarBlockchainLib
AccountModel.cpp
Message.cpp
SocketCommunication.cpp
Wallet.cpp
utils.cpp
Block.cpp
Node.cpp
SocketConnector.cpp
config.cpp
Blockchain.cpp
NodeApi.cpp
Transaction.cpp
Lot.cpp
ProofOfStake.cpp
TransactionPool.cpp
p2putils.cpp)

if(UNIX)
    include_directories(include /usr/local/include /usr/local/opt/openssl@3/include)
    set (OPENSSL_ROOT_DIR /usr/local/opt/openssl@3)
    find_package(OpenSSL REQUIRED)
    find_package(Boost REQUIRED)
    add_executable(Prawnstar main.cpp)

    SET(CMAKE_CXX_FLAGS "-Wall -std=c++14")

    target_link_libraries(Prawnstar PUBLIC PrawnstarBlockchainLib OpenSSL::Crypto)
endif(UNIX)

if (MINGW)
    set(OPENSSL_ROOT_DIR "C:\\OpenSSL-Win64\\")
    include(FindOpenSSL)

    include_directories("C:\\boost\\boost_1_78_0" 
    "include" 
    "C:\\msys64\\mingw64\\x86_64-w64-mingw32\\include"
    "C:\\OpenSSL-Win64\\include")

    find_package(Boost REQUIRED)
    add_executable(Prawnstar main.cpp)

    SET(CMAKE_CXX_FLAGS "-Wall")

    if (OPENSSL_FOUND)
    message(STATUS "OpenSSL Found: ${OPENSSL_VERSION}")
    message(STATUS "OpenSSL Include: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")
    else() 
    message (FATAL_ERROR "OpenSSL Not found.")
    endif()

    target_link_libraries(
    Prawnstar 
    PRIVATE 
    PrawnstarBlockchainLib
    "C:\\OpenSSL-Win64\\bin\\libcrypto-3-x64.dll"
    "C:\\OpenSSL-Win64\\bin\\libssl-3-x64.dll"
    ws2_32
    wsock32
    bcrypt)
endif(MINGW)