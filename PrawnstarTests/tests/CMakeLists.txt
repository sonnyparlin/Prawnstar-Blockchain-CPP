include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(RunTests BlockchainTests.cpp ProofOfStakeTests.cpp)

if (MINGW)
        target_link_libraries(RunTests gtest gtest_main PrawnstarBlockchainLib OpenSSL::Crypto wsock32 ws2_32)
endif(MINGW)

if (UNIX)
        target_link_libraries(RunTests gtest gtest_main PrawnstarBlockchainLib OpenSSL::Crypto)
endif(UNIX)