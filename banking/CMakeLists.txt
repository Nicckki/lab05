cmake_minimum_required(VERSION 3.10)
project(banking)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создаём статическую библиотеку
add_library(banking STATIC
    src/Account.cpp
    src/Transaction.cpp
)

# Указываем пути к заголовочным файлам
target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
