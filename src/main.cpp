// ============================================================================
// main.cpp — точка входа (подход Visitor).
//
// Стандартный конвейер ANTLR4:
//   текст → лексер → токены → парсер → CST → Visitor
//
// Этот конвейер одинаков для любого проекта на ANTLR4.
// Меняется только грамматика и реализация Visitor.
// ============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <any>

#include "antlr4-runtime.h"
#include "LogicGrammarLexer.h"
#include "LogicGrammarParser.h"
#include "LogicInterpreter.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: main <file>" << std::endl;
        return 1;
    }

    // 1. Чтение файла
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // 2. Лексер: текст → поток токенов
    antlr4::ANTLRInputStream inputStream(buffer.str());
    LogicGrammarLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokens(&lexer);

    // 3. Парсер: поток токенов → дерево разбора (CST)
    LogicGrammarParser parser(&tokens);
    LogicGrammarParser::ProgramContext* tree = parser.program();

    // 4. Проверка ошибок разбора
    if (parser.getNumberOfSyntaxErrors() > 0) {
        std::cerr << "Parsing failed with "
                  << parser.getNumberOfSyntaxErrors()
                  << " syntax error(s)." << std::endl;
        return 1;
    }

    // 5. Интерпретатор: обход дерева с вычислением
    try {
        LogicInterpreter interpreter;
        auto result = interpreter.visit(tree);

        if (!result.has_value()) {
            std::cerr << "Error: Failed to evaluate expression" << std::endl;
            return 1;
        }

        bool value = std::any_cast<bool>(result);
        std::cout << (value ? "TRUE" : "FALSE") << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
