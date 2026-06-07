cmake_minimum_required(VERSION 3.10)
project(lab05)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

# Библиотека print (из основной работы)
add_library(print STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/print.cpp)

target_include_directories(print PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Библиотека banking (для ДЗ)
add_subdirectory(banking)

# Подключаем GTest и тесты
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest)
    
    # Тесты для библиотеки print
    file(GLOB PRINT_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/print/*.cpp")
    if(PRINT_TEST_SOURCES)
        add_executable(check_print ${PRINT_TEST_SOURCES})
        target_link_libraries(check_print print gtest_main)
        add_test(NAME check_print COMMAND check_print)
    endif()
    
    # Тесты для библиотеки banking
    file(GLOB BANKING_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/banking/*.cpp")
    if(BANKING_TEST_SOURCES)
        add_executable(check_banking ${BANKING_TEST_SOURCES})
        target_link_libraries(check_banking banking gtest_main)
        add_test(NAME check_banking COMMAND check_banking)
    endif()
endif()
