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
#include <memory>

#include "antlr4-runtime.h"          // основной заголовок ANTLR4 runtime
#include "LogicGrammarLexer.h"       // сгенерированный лексер
#include "LogicGrammarParser.h"      // сгенерированный парсер
#include "LogicInterpreter.h"        // наш Visitor-интерпретатор

/**
 * Транслятор логических выражений на ANTLR4.
 *
 * Использование: main <input_file>
 *
 * При успешном выполнении завершается с кодом 0.
 * При ошибке выводит сообщение об ошибке в stderr
 * и завершается с ненулевым кодом возврата.
 */
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

        if (result.has_value()) {
            auto ast = std::any_cast<std::shared_ptr<ASTNode>>(result);
            if (ast) {
                std::cout << "Expression: " << ast->toString() << std::endl;
                std::cout << "Result: " << (ast->evaluate() ? "TRUE" : "FALSE") << std::endl;
            } else {
                std::cerr << "Error: Failed to parse expression" << std::endl;
                return 1;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
