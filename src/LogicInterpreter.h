#pragma once
#include <any>
#include <memory>
#include "LogicGrammarBaseVisitor.h"
#include "AST.h"

class LogicInterpreter : public LogicGrammarBaseVisitor {
public:
    std::any visitProgram(LogicGrammarParser::ProgramContext* ctx) override;
    std::any visitOrExpr(LogicGrammarParser::OrExprContext* ctx);
    std::any visitOrOp(LogicGrammarParser::OrOpContext* ctx) override;
    std::any visitAndExpr(LogicGrammarParser::AndExprContext* ctx);
    std::any visitAndOp(LogicGrammarParser::AndOpContext* ctx) override;
    std::any visitNotExpr(LogicGrammarParser::NotExprContext* ctx);
    std::any visitNotOp(LogicGrammarParser::NotOpContext* ctx) override;
    std::any visitNotBase(LogicGrammarParser::NotBaseContext* ctx) override;
    std::any visitBaseExpr(LogicGrammarParser::BaseExprContext* ctx);
    std::any visitTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) override;
    std::any visitFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) override;
    std::any visitParens(LogicGrammarParser::ParensContext* ctx) override;
};
