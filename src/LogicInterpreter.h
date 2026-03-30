#pragma once
// ============================================================================
// LogicInterpreter.h — Visitor-интерпретатор.
//
// Наследуется от сгенерированного LogicGrammarBaseVisitor.
// Каждый visit-метод соответствует метке (#) в грамматике.
// Выражения возвращают bool через std::any.
//
// Конвейер: текст → лексер → токены → парсер → CST → Visitor → результат
// ============================================================================

#include <any>
#include "LogicGrammarBaseVisitor.h"

class LogicInterpreter : public LogicGrammarBaseVisitor {
public:
    std::any visitProgram(LogicGrammarParser::ProgramContext* ctx) override;
    std::any visitOrOp(LogicGrammarParser::OrOpContext* ctx) override;
    std::any visitAndOp(LogicGrammarParser::AndOpContext* ctx) override;
    std::any visitNotOp(LogicGrammarParser::NotOpContext* ctx) override;
    std::any visitNotBase(LogicGrammarParser::NotBaseContext* ctx) override;
    std::any visitTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) override;
    std::any visitFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) override;
    std::any visitParens(LogicGrammarParser::ParensContext* ctx) override;
};
