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

include_directories(include /usr/local/include /usr/local/opt/openssl@3/include)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)
add_executable(Prawnstar main.cpp)

SET(CMAKE_CXX_FLAGS "-Wall")

target_link_libraries(Prawnstar PUBLIC PrawnstarBlockchainLib OpenSSL::Crypto)
