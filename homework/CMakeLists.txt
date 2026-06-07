cmake_minimum_required(VERSION 3.4)
project(homework)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(formatter_lib)
add_subdirectory(formatter_ex_lib)
add_subdirectory(solver_lib)

# Указываем пути к заголовочным файлам для hello_world
add_executable(hello_world hello_world_application/hello_world.cpp)
target_include_directories(hello_world PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hello_world formatter_ex)

# Указываем пути к заголовочным файлам для solver_app
add_executable(solver_app solver_application/equation.cpp)
target_include_directories(solver_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(solver_app formatter_ex solver)
