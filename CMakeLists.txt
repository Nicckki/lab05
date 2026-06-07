cmake_minimum_required(VERSION 3.10)
project(lab05)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

# Библиотека print
add_library(print STATIC sources/print.cpp)
target_include_directories(print PUBLIC include)

# Библиотека banking
add_library(banking STATIC
    banking/src/Account.cpp
    banking/src/Transaction.cpp
)
target_include_directories(banking PUBLIC banking/include)

# Тесты
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest)
    
    # Тест для print
    add_executable(check_print tests/print/test_print.cpp)
    target_link_libraries(check_print print gtest_main)
    add_test(NAME check_print COMMAND check_print)
    
    # Тесты для banking (обычные)
    add_executable(check_banking 
        tests/banking/test_account.cpp
        tests/banking/test_transaction.cpp
    )
    target_link_libraries(check_banking banking gtest_main)
    add_test(NAME check_banking COMMAND check_banking)
    
    # Тесты с mock-объектами
    add_executable(check_banking_mock 
        tests/banking/test_transaction_mock.cpp
    )
    target_link_libraries(check_banking_mock banking gtest_main gmock_main)
    add_test(NAME check_banking_mock COMMAND check_banking_mock)
endif()
