# Лабораторная работа №5

Изучение фреймворков тестирования на примере **GTest**

---

## Tutorial

Так как _Travis_ мы не используем, делаем все через _Github Actions_. Также пропускаем ДЗ, так как задание основано на Travis.

Пропустим всю уже привычную преамбулу и сразу приступим к командам, непосредственно относящимся к лабе.

Создаем папку для сторонних библиотек и клонируем туда GTest.

```bash
mkdir third-party
git submodule add https://github.com/google/googletest third-party/gtest
```

<details> 
  <summary>Вывод git submodule add</summary>

    Cloning into '/home/vboxuser/workspace/lab05/third-party/gtest'...
    remote: Enumerating objects: 28670, done.
    remote: Counting objects: 100% (89/89), done.
    remote: Compressing objects: 100% (67/67), done.
    remote: Total 28670 (delta 49), reused 23 (delta 22), pack-reused 28581 (from 3)
    Receiving objects: 100% (28670/28670), 13.84 MiB | 1.69 MiB/s, done.
    Resolving deltas: 100% (21290/21290), done.
</details>

Переключимся на конкретную версию GTest (v1.16.0).

```bash
cd third-party/gtest
git checkout v1.16.0
```

<details> 
  <summary>Вывод git checkout</summary>
  
    Note: switching to 'v1.16.0'.
    
    You are in 'detached HEAD' state. You can look around, make experimental
    changes and commit them, and you can discard any commits you make in this
    state without impacting any branches by switching back to a branch.
    
    HEAD is now at 6910c9d9 Prepare for v1.16.0 (#4721)
</details>

```bash
cd ../..
git add .gitmodules third-party/gtest
git commit -m "added gtest v1.16.0 framework"
```

<details> 
  <summary>Вывод git commit</summary>

    [main 7dd8fa2] added gtest v1.16.0 framework
     2 files changed, 4 insertions(+)
     create mode 100644 .gitmodules
     create mode 160000 third-party/gtest
</details>

## Настройка CMakeLists.txt

Изменим версию C++ стандарта на 14 (требуется для GTest).

```bash
sed -i 's/CMAKE_CXX_STANDARD 11/CMAKE_CXX_STANDARD 14/g' CMakeLists.txt
```

Итоговое содержимое CMakeLists.txt:

```bash
cmake_minimum_required(VERSION 3.10)
project(print)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

add_library(print STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/print.cpp)

target_include_directories(print PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
  add_executable(check ${TEST_SOURCES})
  target_link_libraries(check print gtest_main)
  add_test(NAME check COMMAND check)
endif()

install(TARGETS print
    EXPORT print-config
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install(EXPORT print-config DESTINATION cmake)
```

## Создание теста

Создадим папку для тестов и файл с тестами.

```bash
mkdir -p tests
nano tests/test1.cpp
```

Содержимое tests/test1.cpp:

```bash#include <print.hpp>
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <cstdio>

TEST(Print, InFileStream)
{
    std::string filepath = "/tmp/test_output.txt";
    std::string text = "Hello, GTest!";
    
    std::ofstream out{filepath};
    ASSERT_TRUE(out.is_open()) << "Failed to open file for writing";
    print(text, out);
    out.close();
    
    std::string result;
    std::ifstream in{filepath};
    ASSERT_TRUE(in.is_open()) << "Failed to open file for reading";
    std::getline(in, result);
    in.close();
    
    EXPECT_EQ(result, text);
    
    std::remove(filepath.c_str());
}

TEST(Print, ToStringStream)
{
    std::stringstream buffer;
    std::string text = "Testing stringstream";
    
    print(text, buffer);
    
    std::string result = buffer.str();
    EXPECT_EQ(result, text);
}
```

## Сборка проекта и запуск тестов

```bash
rm -rf build
cmake -H. -B build -DBUILD_TESTS=ON
```

<details> 
  <summary>Вывод cmake</summary>

    -- The C compiler identification is GNU 11.4.0
    -- The CXX compiler identification is GNU 11.4.0
    -- Detecting C compiler ABI info
    -- Detecting C compiler ABI info - done
    -- Check for working C compiler: /usr/bin/cc - skipped
    -- Detecting C compile features
    -- Detecting C compile features - done
    -- Detecting CXX compiler ABI info
    -- Detecting CXX compiler ABI info - done
    -- Check for working CXX compiler: /usr/bin/c++ - skipped
    -- Detecting CXX compile features
    -- Detecting CXX compile features - done
    -- Looking for pthread.h
    -- Looking for pthread.h - found
    -- Performing Test CMAKE_HAVE_LIBC_PTHREAD
    -- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
    -- Found Threads: TRUE  
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /home/vboxuser/workspace/lab05/build
</details>

```bash
cmake --build build
```

<details> 
  <summary>Вывод cmake --build build</summary>

    [  8%] Building CXX object CMakeFiles/print.dir/sources/print.cpp.o
    [ 16%] Linking CXX static library libprint.a
    [ 16%] Built target print
    [ 25%] Building CXX object third-party/gtest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
    [ 33%] Linking CXX static library ../../../lib/libgtest.a
    [ 33%] Built target gtest
    [ 41%] Building CXX object third-party/gtest/googletest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
    [ 50%] Linking CXX static library ../../../lib/libgtest_main.a
    [ 50%] Built target gtest_main
    [ 58%] Building CXX object CMakeFiles/check.dir/tests/test1.cpp.o
    [ 66%] Linking CXX executable check
    [ 66%] Built target check
    [ 75%] Building CXX object third-party/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
    [ 83%] Linking CXX static library ../../../lib/libgmock.a
    [ 83%] Built target gmock
    [ 91%] Building CXX object third-party/gtest/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
    [100%] Linking CXX static library ../../../lib/libgmock_main.a
    [100%] Built target gmock_main
</details>

Запускаем тесты.

```bash
cd build                    # Переходим в папку сборки
ctest --output-on-failure   # Запускаем тесты через CTest
```

<details> 
  <summary>Вывод ctest</summary>
    
    Test project /home/vboxuser/workspace/lab05/build
        Start 1: check
    1/1 Test #1: check ............................   Passed    0.01 sec
    
    100% tests passed, 0 tests failed out of 1
</details>

Запускаем тесты напрямую

```bash
./check
```

<details> 
  <summary>Вывод ./check</summary>

    Running main() from /home/vboxuser/workspace/lab05/third-party/gtest/googletest/src/gtest_main.cc
    [==========] Running 2 tests from 1 test suite.
    [----------] Global test environment set-up.
    [----------] 2 tests from Print
    [ RUN      ] Print.InFileStream
    [       OK ] Print.InFileStream (1 ms)
    [ RUN      ] Print.ToStringStream
    [       OK ] Print.ToStringStream (0 ms)
    [----------] 2 tests from Print (1 ms total)
    
    [----------] Global test environment tear-down
    [==========] 2 tests from 1 test suite ran. (1 ms total)
    [  PASSED  ] 2 tests.
</details>

## Настройка GitHub Actions

Создаём конфигурацию для непрерывной интеграции (CI) на GitHub.

Файл .github/workflows/linux.yml:

```bash
name: C++ CI with Tests     # Название workflow

on: [push, pull_request]    # Запускать при push и pull request

jobs:
  build-and-test:           # Имя задания
    runs-on: ubuntu-latest  # Используем последнюю версию Ubuntu
    
    steps:
    - uses: actions/checkout@v4  # Клонируем репозиторий
      with:
        submodules: recursive    # Загружаем submodule (GTest)
    
    - name: Configure CMake   # Настраиваем CMake
      run: cmake -H. -B build -DBUILD_TESTS=ON
    
    - name: Build             # Собираем проект
      run: cmake --build build
    
    - name: Run tests         # Запускаем тесты
      run: ctest --test-dir build --output-on-failure
```

## Фиксация изменений в репозитории

Добавляем все изменения, создаём коммит и отправляем на GitHub.

```bash
cd ~/workspace/lab05        # Переходим в корень проекта
git add -A                  # Добавляем все изменения
git commit -m "add working tests and configure CI"  # Создаём коммит
```

<details> 
  <summary>Вывод git commit</summary>

    [main 6717b74] add working tests and configure CI
     16 files changed, 88 insertions(+), 146 deletions(-)
     rewrite CMakeLists.txt (62%)
     delete mode 100644 appveyor.yml
     delete mode 100644 hello_world.cpp
     delete mode 100644 homework/CMakeLists.txt
     delete mode 100644 homework/formatter_ex_lib/CMakeLists.txt
     delete mode 100644 homework/formatter_ex_lib/formatter_ex.cpp
     delete mode 100644 homework/formatter_ex_lib/formatter_ex.h
     delete mode 100644 homework/formatter_lib/CMakeLists.txt
     delete mode 100644 homework/formatter_lib/formatter.cpp
     delete mode 100644 homework/formatter_lib/formatter.h
     delete mode 100644 homework/hello_world_application/hello_world.cpp
     delete mode 100644 homework/solver_application/equation.cpp
     delete mode 100644 homework/solver_lib/CMakeLists.txt
     delete mode 100644 homework/solver_lib/solver.cpp
     delete mode 100644 homework/solver_lib/solver.h
     create mode 100644 tests/test1.cpp
</details>

```bash
git push origin main
```

<details> 
  <summary>Вывод git push</summary>

    Enumerating objects: 11, done.
    Counting objects: 100% (11/11), done.
    Compressing objects: 100% (7/7), done.
    Writing objects: 100% (9/9), 1.97 KiB | 673.00 KiB/s, done.
    Total 9 (delta 2), reused 0 (delta 0), pack-reused 0
    remote: Resolving deltas: 100% (2/2), completed with 1 local object.
    To https://github.com/Nicckki/lab05.git
       4fa562e..6717b74  main -> main
</details>

## Вывод

В ходе работы был освоен фреймворк Google Test и процесс настройки автоматического тестирования C++ проекта с использованием современного инструмента GitHub Actions, который полностью заменил устаревший и платный Travis CI.
