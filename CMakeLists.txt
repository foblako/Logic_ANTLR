cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(antlr4_lab LANGUAGES CXX)

# C++17 обязателен для antlr4-runtime (std::any, std::string_view)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ────────────────────────────────────────────────────────────────────────────
# 1. Подключение ANTLR4 C++ Runtime
#
#    Стратегия:
#      a) Ищем предустановленный runtime
#      b) Если не найден — скачиваем через FetchContent (локальная разработка)
# ────────────────────────────────────────────────────────────────────────────
find_library(ANTLR4_LIB antlr4-runtime)
find_path(ANTLR4_INCLUDE antlr4-runtime.h PATH_SUFFIXES antlr4-runtime)

if(ANTLR4_LIB AND ANTLR4_INCLUDE)
    message(STATUS "Found pre-installed ANTLR4 runtime: ${ANTLR4_LIB}")
    message(STATUS "ANTLR4 headers: ${ANTLR4_INCLUDE}")

    add_library(antlr4_static STATIC IMPORTED)
    set_target_properties(antlr4_static PROPERTIES
        IMPORTED_LOCATION "${ANTLR4_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${ANTLR4_INCLUDE}"
    )
else()
    message(STATUS "Pre-installed ANTLR4 runtime not found — using FetchContent")
    message(STATUS "(первая сборка займёт некоторое время — компилируется ANTLR4 runtime)")

    include(FetchContent)

    set(ANTLR4_WITH_STATIC_CRT OFF CACHE BOOL "" FORCE)
    set(ANTLR_BUILD_CPP_TESTS OFF CACHE BOOL "" FORCE)
    set(ANTLR_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(DISABLE_WARNINGS ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        antlr4-runtime
        GIT_REPOSITORY https://github.com/antlr/antlr4.git
        GIT_TAG        dev
        SOURCE_SUBDIR  runtime/Cpp
    )
    FetchContent_MakeAvailable(antlr4-runtime)
endif()

# ────────────────────────────────────────────────────────────────────────────
# 2. ANTLR4 Tool (JAR-файл для генерации парсера из .g4)
#    Java нужна ТОЛЬКО на этапе генерации.
#    В итоговом бинарнике Java не участвует.
#
#    Если в окружении задан ANTLR_JAR_PATH (Docker), используем его.
#    Иначе скачиваем в build/.
# ────────────────────────────────────────────────────────────────────────────
find_package(Java COMPONENTS Runtime REQUIRED)

if(DEFINED ENV{ANTLR_JAR_PATH} AND EXISTS "$ENV{ANTLR_JAR_PATH}")
    set(ANTLR_JAR "$ENV{ANTLR_JAR_PATH}")
    message(STATUS "Using pre-downloaded ANTLR JAR: ${ANTLR_JAR}")
else()
    set(ANTLR_JAR ${CMAKE_CURRENT_BINARY_DIR}/antlr-4.13.2-complete.jar)
    if(NOT EXISTS "${ANTLR_JAR}")
        message(STATUS "Downloading ANTLR 4.13.2 JAR...")
        file(DOWNLOAD
            "https://www.antlr.org/download/antlr-4.13.2-complete.jar"
            "${ANTLR_JAR}"
            SHOW_PROGRESS
        )
    endif()
endif()

# ────────────────────────────────────────────────────────────────────────────
# 3. Генерация парсера из LogicGrammar.g4 (чистая грамматика, для Visitor)
# ────────────────────────────────────────────────────────────────────────────
set(GRAMMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/grammar/LogicGrammar.g4)
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(ANTLR_GENERATED_SOURCES
    ${GENERATED_DIR}/LogicGrammarLexer.cpp
    ${GENERATED_DIR}/LogicGrammarParser.cpp
    ${GENERATED_DIR}/LogicGrammarBaseVisitor.cpp
    ${GENERATED_DIR}/LogicGrammarBaseListener.cpp
    ${GENERATED_DIR}/LogicGrammarVisitor.cpp
    ${GENERATED_DIR}/LogicGrammarListener.cpp
)

add_custom_command(
    OUTPUT ${ANTLR_GENERATED_SOURCES}
    COMMAND ${Java_JAVA_EXECUTABLE} -jar ${ANTLR_JAR}
        -Dlanguage=Cpp
        -visitor
        -listener
        -o ${GENERATED_DIR}
        ${GRAMMAR_FILE}
    DEPENDS ${GRAMMAR_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating ANTLR4 C++ parser from LogicGrammar.g4"
)

# ────────────────────────────────────────────────────────────────────────────
# 4. Основной исполняемый файл (Visitor-подход)
# ────────────────────────────────────────────────────────────────────────────
add_executable(main
    src/main.cpp
    src/AST.cpp
    src/LogicInterpreter.cpp
    ${ANTLR_GENERATED_SOURCES}
)

target_include_directories(main PRIVATE
    ${GENERATED_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(main PRIVATE antlr4_static)
