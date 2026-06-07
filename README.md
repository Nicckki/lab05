# Лабораторная работа №4

Данная лабораторная работа посвещена изучению систем непрерывной интеграции.

---

## Домашнее задание

Так как _Travis_ мы не используем, сразу переходим к ДЗ и делаем все через _Github Actions_ для линукса и _Appveyor_ на винде.

Рассмотрим сначала файл для Github Actions:

```bash
name: linux
on: push
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - name: Get repo files
        uses: actions/checkout@v4
      - name: Configure build directory
        run: cmake -H. -B build
      - name: Build project
        run: cmake --build build
      - name: Upload artifacts
        uses: actions/upload-artifact@v4
        with: 
          name: Applications
          path: |
            ./build/hello_world
            ./build/solver_app
  test:
    needs: build
    runs-on: ubuntu-latest
    steps:
      - name: Download artifacts
        uses: actions/download-artifact@v4
        with:
          name: Applications
          path: ./apps
      - name: Fix permissions
        run: |
          chmod +x ./apps/hello_world
          chmod +x ./apps/solver_app
      - name: Test hello_world
        run: ./apps/hello_world
      - name: Test solver
        run: echo "1 5 -6" | ./apps/solver_app
```

Я разбил workflow на два jobs , один для сборки, другой для первичного тестирования. Чтобы получить доступ к файлам из репозитория, использовал actions/checkout@v4, а чтобы передать собранные приложения между jobs использовал actions/upload-artifact@v4 и actions/download-artifact@v4. Также стоит отметить, что сначала в test приложения не запускались из-за отсутствия разрешения, поэтому пришлось добавить команды chmod +x.

Теперь рассмотрим appveyor.yml:

```bash
version: 1.0.{build}

image:
  - Visual Studio 2022
  - Visual Studio 2019

platform:
  - x64
  - x86

configuration:
  - Debug
  - Release

matrix:
  fast_finish: true

build_script:
  - cmake -B build -DCMAKE_BUILD_TYPE=%CONFIGURATION%
  - cmake --build build --config %CONFIGURATION%

test_script:
  - .\build\%CONFIGURATION%\hello_world.exe
  - echo 1 5 -6 | .\build\%CONFIGURATION%\solver_app.exe
```

Здесь я провожу тесты в компиляторах Visual Studio 2022 и Visual Studio 2019, на платформах x64 и x86, в режимах debug и release. Также стоит отметить, что при компиляции в appveyor возникла ошибка, что якобы sqrt не является членом std. Выяснилось, что на компиляторе MSVS нужно подключать cmath, а не math.h. Поэтому пришлось внести следующее изменение в solver.cpp:

```bash
#ifdef _WIN32
    #include <cmath>
#else
    #include <math.h>
#endif
```

которое подключает одну из библиотек в зависимости от ОС.


