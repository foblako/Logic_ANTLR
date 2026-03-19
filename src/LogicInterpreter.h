#pragma once
#include <any>
#include <memory>
#include "LogicGrammarBaseVisitor.h"
#include "AST.h"

class LogicInterpreter : public LogicGrammarBaseVisitor {
public:
    std::any visitOrOp(LogicGrammarParser::OrOpContext* ctx) override;
    std::any visitAndOp(LogicGrammarParser::AndOpContext* ctx) override;
    std::any visitNotOp(LogicGrammarParser::NotOpContext* ctx) override;
    std::any visitNotBase(LogicGrammarParser::NotBaseContext* ctx) override;
    std::any visitTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) override;
    std::any visitFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) override;
    std::any visitParens(LogicGrammarParser::ParensContext* ctx) override;
};
