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

## Установка зависимостей

### Windows

1. **Установите компилятор C++** (один из вариантов):

   **Вариант A: Visual Studio Build Tools** (рекомендуется)
   - Скачайте с [visualstudio.microsoft.com/visual-cpp-build-tools/](https://visualstudio.microsoft.com/visual-cpp-build-tools/)
   - Установите компонент **"C++ build tools"**
   - Для сборки используйте **"x64 Native Tools Command Prompt for VS"**

   **Вариант B: MinGW-w64**
   - Установите через [MSYS2](https://www.msys2.org/) или [winlibs.com](https://winlibs.com/)
   - Добавьте путь к `bin` в переменную окружения `PATH`

2. **Установите остальные зависимости:**
   ```powershell
   winget install -ie --id Kitware.CMake
   winget install -ie --id EclipseAdoptium.Temurin.17.JRE
   winget install -ie --id Git.Git
   ```

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install -y build-essential cmake git openjdk-17-jre-headless pkg-config uuid-dev
```

## Сборка

**Первая сборка занимает несколько минут** (скачивается и компилируется ANTLR4 runtime).

### Windows

**Для Visual Studio Build Tools** (запускать из "x64 Native Tools Command Prompt for VS"):
```powershell
cmake -B build
cmake --build build --config Release
```

**Для MinGW:**
```powershell
cmake -G "MinGW Makefiles" -B build
cmake --build build
```

### Linux

```bash
cmake -B build
cmake --build build
```

## Запуск

### Windows

**Visual Studio Build Tools:**
```powershell
.\build\Release\main.exe .\examples\valid\01_true.example
```

**MinGW:**
```powershell
.\build\main.exe .\examples\valid\01_true.example
```

### Linux / WSL

```bash
./build/main ./examples/valid/01_true.example
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
