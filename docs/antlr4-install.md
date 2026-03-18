# Установка окружения для ANTLR4 (C++ target)

Для сборки проекта необходимы три компонента: **CMake** (система сборки), **Java JRE** (только для генерации парсера из `.g4`), и **компилятор C++17** (GCC, Clang, или MSVC).

Java нужна **исключительно** на этапе генерации кода — ANTLR Tool (`antlr-4.13.2-complete.jar`) написан на Java. В итоговом исполняемом файле Java не участвует: только ваш C++ код и C++ runtime-библиотека ANTLR4.

ANTLR4 C++ runtime и JAR-файл скачиваются **автоматически** при первой сборке через CMake (FetchContent). Ручная установка ANTLR4 **не требуется**.

---

## Windows

### 1. CMake

```powershell
winget install -ie --id Kitware.CMake
```

Проверка (в новом окне PowerShell):

```powershell
cmake --version    # нужна 3.14+
```

### 2. Java JRE

```powershell
winget install -ie --id EclipseAdoptium.Temurin.17.JRE
```

Если `winget` недоступен — скачайте JRE 17+ с сайта https://www.java.com/ru/download/manual.jsp и установите вручную.

Проверка:

```powershell
java -version    # нужна 11+
```

### 3. Компилятор C++

Если установлена **Visual Studio 2019+** с компонентом «Desktop development with C++» — компилятор уже есть, дополнительных действий не требуется.

Если Visual Studio нет, установите Build Tools и Clang:

```powershell
winget install -ie --id Microsoft.VisualStudio.2022.BuildTools
```

В открывшемся установщике выберите **«Desktop development with C++»** и нажмите **Install**.

### 4. Git

CMake FetchContent использует Git для скачивания ANTLR4 runtime. Убедитесь, что Git установлен:

```powershell
winget install -ie --id Git.Git
```

### 5. Сборка проекта

```powershell
cmake -B build
cmake --build build --config Release
```

Первая сборка занимает 3–7 минут (скачивание и компиляция ANTLR4 runtime). Последующие — секунды.

Запуск:

```powershell
.\build\Release\main.exe .\examples\valid\01_true.example
```

---

## Linux (Ubuntu / Debian)

### 1. Все зависимости одной командой

```bash
sudo apt update
sudo apt install -y build-essential cmake git openjdk-17-jre-headless pkg-config uuid-dev
```

Проверка:

```bash
java -version     # нужна 11+
cmake --version   # нужна 3.14+
g++ --version     # нужен C++17 (GCC 7+)
```

### 2. Fedora / RHEL

```bash
sudo dnf install gcc-c++ cmake git java-17-openjdk-headless pkg-config libuuid-devel
```

### 3. Сборка проекта

```bash
cmake -B build
cmake --build build -j$(nproc)
```

Первая сборка занимает 3–7 минут. Последующие — секунды.

Запуск:

```bash
./build/main ./examples/valid/01_true.example
```

---

## Опционально: установка ANTLR4 Tool (для отладки грамматик)

Это **не обязательно** для сборки проекта — CMake скачивает JAR автоматически. Но утилита `antlr4-parse` полезна для быстрой проверки грамматики без компиляции C++ кода.

```bash
pip install antlr4-tools
```

После установки доступны две команды:

`antlr4` — генерация парсера (обёртка над `java -jar antlr-4.13.2-complete.jar`).

`antlr4-parse` — тестирование грамматики без компиляции. Позволяет увидеть дерево разбора, поток токенов и трассировку парсера прямо в терминале.

Примеры использования:

```bash
# Поток токенов
echo 'TRUE && FALSE' | antlr4-parse grammar/LogicGrammar.g4 program -tokens

# Дерево разбора (текст)
echo 'TRUE && FALSE' | antlr4-parse grammar/LogicGrammar.g4 program -tree

# Графическое дерево (нужен GUI)
echo 'TRUE && FALSE' | antlr4-parse grammar/LogicGrammar.g4 program -gui
```
