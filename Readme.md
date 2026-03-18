# Лабораторная работа: транслятор на ANTLR4

## Что такое ANTLR4

**ANTLR4** (ANother Tool for Language Recognition) — генератор лексеров и парсеров. По файлу с грамматикой (`.g4`) он генерирует C++ код, который читает текст, разбивает его на токены и строит дерево разбора.

В отличие от Bison/Flex, где лексер и парсер описываются в двух раздельных файлах (`.l` и `.y`), в ANTLR4 вся грамматика — в одном файле `.g4`. Парсер строит полное дерево разбора (CST) **автоматически**, а семантика реализуется отдельным обходом этого дерева.

ANTLR Tool написан на Java, но Java нужна **только при генерации** кода. В итоговом бинарнике — только C++ и runtime-библиотека ANTLR4.

Инструкции по установке всех зависимостей — в файле [docs/antlr4-install.md](docs/antlr4-install.md).

---

## Структура проекта

```
.
├── CMakeLists.txt                  # сборочный скрипт (CMake + FetchContent)
├── grammar/
│   └── LogicGrammar.g4             # грамматика (чистая, для Visitor/Listener)
├── src/
│   ├── main.cpp                    # точка входа (подход Visitor)
│   ├── LogicInterpreter.h/.cpp     # Visitor-интерпретатор
│   └── AST.h/.cpp                  # узлы AST
├── examples/
│   ├── valid/                      # положительные примеры
│   ├── invalid/                    # негативные примеры
│   └── run_tests.sh                # скрипт автоматической проверки
├── docker/
│   ├── Dockerfile                  # контейнер со всем окружением
│   ├── docker-entrypoint.sh        # точка входа контейнера
│   └── docker-compose.yml          # конфигурация Docker Compose
└── docs/
    └── antlr4-install.md           # инструкции по установке
```

## Подход к реализации семантики

В проекте используется **Visitor-подход** (файлы `LogicGrammar.g4` + `main.cpp` + `LogicInterpreter.h` + `AST.h`). Грамматика описывает только синтаксис. Семантика реализуется в отдельном C++ классе, который наследуется от сгенерированного `LogicGrammarBaseVisitor` и переопределяет `visit`-методы. Это рекомендуемый подход для серьёзных проектов: грамматика остаётся чистым описанием языка, код модульный и тестируемый.

---

## Язык логических выражений

Язык: `TRUE`, `FALSE`, `!`, `&&`, `||`, `()`

Примеры:
- `TRUE`
- `!TRUE`
- `TRUE && FALSE`
- `!TRUE && (!!FALSE)`
- `(TRUE || FALSE) && !(!FALSE)`

Приоритет операторов: `()` → `!` → `&&` → `||`

---

## Задание

Разработать транслятор для языка логических выражений, используя ANTLR4 с целевым языком C++17.

Транслятор должен:

1. Принимать путь к файлу с программой в качестве **аргумента командной строки**.
2. При успешном разборе и выполнении — выводить результат и завершаться с кодом возврата **0**.
3. При ошибке (синтаксической или семантической) — выводить сообщение об ошибке в `stderr` и завершаться с **ненулевым** кодом возврата.

```bash
# Правильно:
./build/main my_program.txt

# Неправильно:
./build/main < my_program.txt
```

---

## Требования к сдаче

### Грамматика

Файл `.g4` с грамматикой вашего языка в директории `grammar/`. Грамматика должна покрывать все конструкции языка из вашего варианта.

### Семантика

Реализация семантических действий одним из способов: Visitor, Listener, или встроенные действия в грамматике.

### Примеры программ

При сдаче обязательно приложите набор примеров в папке `examples/`.

**Положительные примеры** (`examples/valid/`) — корректные программы, которые транслятор должен принять и обработать без ошибок. Примеры должны покрывать все ключевые конструкции языка.

**Негативные примеры** (`examples/invalid/`) — программы с намеренными ошибками, на которых транслятор должен сообщить об ошибке и завершиться с ненулевым кодом возврата. Каждый файл должен содержать комментарий, объясняющий, какая именно ошибка в нём содержится.

Пример структуры:

```
examples/
├── valid/
│   ├── 01_true.example       # константа TRUE
│   ├── 03_not_true.example   # отрицание !TRUE
│   └── 09_nested.example     # вложенные скобки
└── invalid/
│   ├── 01_unknown_token.example    # ошибка: неизвестный токен MAYBE
│   ├── 03_unbalanced_parens.example # ошибка: несбалансированные скобки
│   └── 04_empty_parens.example     # ошибка: пустые скобки
```

---

## Сборка через CMake

Убедитесь, что Java JRE и CMake установлены (см. [docs/antlr4-install.md](docs/antlr4-install.md)), затем выполните:

```bash
cmake -B build
cmake --build build
```

Первая сборка занимает 3–7 минут: CMake автоматически скачает ANTLR4 C++ runtime (~100 МБ), JAR-файл ANTLR Tool (~2 МБ), генерирует парсер из `.g4` и компилирует всё. Последующие пересборки — секунды.

**Запуск на Linux / WSL:**

```bash
./build/main ./examples/valid/01_true.example
```

**Запуск на Windows:**

```powershell
.\build\Release\main.exe .\examples\valid\01_true.example
```

### Что собирается

CMake создаёт один исполняемый файл:

`main` — Visitor-подход (грамматика `LogicGrammar.g4` + интерпретатор `LogicInterpreter.h` + AST).

---

## Автоматическая проверка примеров

Скрипт `examples/run_tests.sh` прогоняет все файлы из `examples/valid/` и `examples/invalid/` через транслятор. Для `valid/` ожидается код возврата 0, для `invalid/` — ненулевой.

```bash
chmod +x examples/run_tests.sh
./examples/run_tests.sh ./build/main
```

Пример вывода:

```
Транслятор: ./build/main
──────────────────────────────────────────
Положительные примеры (valid/):
  PASS  valid/01_true.example
  PASS  valid/08_complex.example

Негативные примеры (invalid/):
  PASS  invalid/01_unknown_token.example
  PASS  invalid/03_unbalanced_parens.example

──────────────────────────────────────────
Итог: 13 прошло / 0 упало / 13 всего
```

---

## Сборка и запуск через Docker

Самый простой способ запустить проект без установки чего-либо вручную — использовать Docker.

```bash
# Сборка образа и запуск тестов
docker compose -f docker/docker-compose.yml build tests
docker compose -f docker/docker-compose.yml run --rm tests

# Запуск на конкретном файле
docker compose -f docker/docker-compose.yml run --rm translator run /workspace/examples/valid/08_complex.example
```

Первая сборка Docker-образа занимает больше времени (установка пакетов + компиляция ANTLR4 runtime). Последующие запуски используют кеш.

---

## Как начать работу

1. Склонируйте этот репозиторий.
2. Убедитесь, что сборка проходит: `cmake -B build && cmake --build build`.
3. Запустите пример: `./build/main ./examples/valid/01_true.example` — должно вывести `TRUE`.
4. Изучите файлы `grammar/LogicGrammar.g4`, `src/LogicInterpreter.h` и `src/main.cpp`.
5. Добавьте примеры в `examples/valid/` и `examples/invalid/`.
6. Убедитесь, что `./examples/run_tests.sh ./build/main` проходит.

### Быстрая проверка грамматики без компиляции

Если установлена утилита `antlr4-parse` (см. [docs/antlr4-install.md](docs/antlr4-install.md)), можно проверять грамматику без сборки C++ кода:

```bash
# Дерево разбора
echo 'TRUE && FALSE' | antlr4-parse grammar/LogicGrammar.g4 program -tree

# Поток токенов
echo 'TRUE && FALSE' | antlr4-parse grammar/LogicGrammar.g4 program -tokens
```

---
