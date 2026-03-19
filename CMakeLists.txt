cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(antlr4_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(ANTLR4_LIB antlr4-runtime)
find_path(ANTLR4_INCLUDE antlr4-runtime.h PATH_SUFFIXES antlr4-runtime)

if(ANTLR4_LIB AND ANTLR4_INCLUDE)
    message(STATUS "Found ANTLR4 runtime: ${ANTLR4_LIB}")
    add_library(antlr4_static STATIC IMPORTED)
    set_target_properties(antlr4_static PROPERTIES
        IMPORTED_LOCATION "${ANTLR4_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${ANTLR4_INCLUDE}"
    )
else()
    message(STATUS "ANTLR4 runtime not found, downloading...")
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

find_package(Java COMPONENTS Runtime REQUIRED)

if(DEFINED ENV{ANTLR_JAR_PATH} AND EXISTS "$ENV{ANTLR_JAR_PATH}")
    set(ANTLR_JAR "$ENV{ANTLR_JAR_PATH}")
else()
    set(ANTLR_JAR ${CMAKE_CURRENT_BINARY_DIR}/antlr-4.13.2-complete.jar)
    if(NOT EXISTS "${ANTLR_JAR}")
        message(STATUS "Downloading ANTLR JAR...")
        file(DOWNLOAD "https://www.antlr.org/download/antlr-4.13.2-complete.jar" "${ANTLR_JAR}" SHOW_PROGRESS)
    endif()
endif()

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
        -Dlanguage=Cpp -visitor -listener -o ${GENERATED_DIR} ${GRAMMAR_FILE}
    DEPENDS ${GRAMMAR_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating ANTLR4 sources"
)

add_executable(main
    src/main.cpp
    src/AST.cpp
    src/LogicInterpreter.cpp
    ${ANTLR_GENERATED_SOURCES}
)

target_include_directories(main PRIVATE ${GENERATED_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(main PRIVATE antlr4_static)
