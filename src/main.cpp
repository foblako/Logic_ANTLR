#include <iostream>
#include <fstream>
#include <sstream>
#include <any>
#include <memory>

#include "antlr4-runtime.h"
#include "LogicGrammarLexer.h"
#include "LogicGrammarParser.h"
#include "LogicInterpreter.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: main <file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    antlr4::ANTLRInputStream inputStream(buffer.str());
    LogicGrammarLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokens(&lexer);
    LogicGrammarParser parser(&tokens);
    LogicGrammarParser::ProgramContext* tree = parser.program();

    if (parser.getNumberOfSyntaxErrors() > 0) {
        std::cerr << "Parsing failed with " << parser.getNumberOfSyntaxErrors() << " syntax error(s)." << std::endl;
        return 1;
    }

    try {
        LogicInterpreter interpreter;
        auto result = interpreter.visit(tree);
        auto ast = std::any_cast<std::shared_ptr<ASTNode>>(result);

        if (ast) {
            std::cout << "Expression: " << ast->toString() << std::endl;
            std::cout << "Result: " << (ast->evaluate() ? "TRUE" : "FALSE") << std::endl;
        } else {
            std::cerr << "Error: Failed to parse expression" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
