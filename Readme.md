# Лабораторная работа: транслятор на ANTLR4

## Задание

Разработать транслятор для языка логических выражений, используя ANTLR4 с целевым языком C++17.

Язык: `TRUE`, `FALSE`, `!`, `&&`, `||`, `()`

Примеры:
- `TRUE`
- `!TRUE`
- `TRUE && FALSE`
- `(TRUE || FALSE) && !(!FALSE)`

Приоритет операторов: `()` → `!` → `&&` → `||`

## Требования

Транслятор должен:
1. Принимать путь к файлу с программой в качестве **аргумента командной строки**.
2. При успешном разборе — выводить результат и завершаться с кодом возврата **0**.
3. При ошибке — выводить сообщение в `stderr` и завершаться с **ненулевым** кодом возврата.

```bash
./build/main my_program.txt
```

## Сборка

Убедитесь, что установлены Java JRE и CMake, затем:

```bash
cmake -B build
cmake --build build
```

Первая сборка занимает несколько минут (скачивается и компилируется ANTLR4 runtime).

**Запуск:**

```bash
# Linux / WSL
./build/main ./examples/valid/01_true.example

# Windows
.\build\Release\main.exe .\examples\valid\01_true.example
```

## Автоматическая проверка

```bash
chmod +x examples/run_tests.sh
./examples/run_tests.sh ./build/main
```

## Структура проекта

```
.
├── CMakeLists.txt
├── grammar/
│   └── LogicGrammar.g4
├── src/
│   ├── main.cpp
│   ├── LogicInterpreter.h/.cpp
│   └── AST.h/.cpp
└── examples/
    ├── valid/
    └── invalid/
```

## Установка зависимостей

**Windows:**
```powershell
winget install -ie --id Kitware.CMake
winget install -ie --id EclipseAdoptium.Temurin.17.JRE
winget install -ie --id Git.Git
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install -y build-essential cmake git openjdk-17-jre-headless pkg-config uuid-dev
```
