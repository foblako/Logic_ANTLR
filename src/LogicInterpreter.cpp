#include "LogicInterpreter.h"

// ============================================================================
// visitProgram — стартовая точка.
// program : orExpr EOF, поэтому просто посещаем orExpr.
// ============================================================================
std::any LogicInterpreter::visitProgram(LogicGrammarParser::ProgramContext* ctx) {
    return visit(ctx->orExpr());
}

// ============================================================================
// visitOrOp — операция OR (||).
// orExpr : andExpr (OR_OP andExpr)*
// Вычисляем логическое ИЛИ всех операндов.
// ============================================================================
std::any LogicInterpreter::visitOrOp(LogicGrammarParser::OrOpContext* ctx) {
    bool result = false;
    for (auto* andExpr : ctx->andExpr()) {
        bool value = std::any_cast<bool>(visit(andExpr));
        result = result || value;
    }
    return result;
}

// ============================================================================
// visitAndOp — операция AND (&&).
// andExpr : notExpr (AND_OP notExpr)*
// Вычисляем логическое И всех операндов.
// ============================================================================
std::any LogicInterpreter::visitAndOp(LogicGrammarParser::AndOpContext* ctx) {
    bool result = true;
    for (auto* notExpr : ctx->notExpr()) {
        bool value = std::any_cast<bool>(visit(notExpr));
        result = result && value;
    }
    return result;
}

// ============================================================================
// visitNotOp — операция NOT (!).
// notExpr : NOT_OP notExpr
// Инвертируем результат.
// ============================================================================
std::any LogicInterpreter::visitNotOp(LogicGrammarParser::NotOpContext* ctx) {
    bool value = std::any_cast<bool>(visit(ctx->notExpr()));
    return !value;
}

// ============================================================================
// visitNotBase — базовый случай для notExpr.
// notExpr : baseExpr
// Просто передаём управление дальше.
// ============================================================================
std::any LogicInterpreter::visitNotBase(LogicGrammarParser::NotBaseContext* ctx) {
    return visit(ctx->baseExpr());
}

// ============================================================================
// visitTrueLiteral — литерал TRUE.
// Возвращаем true.
// ============================================================================
std::any LogicInterpreter::visitTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) {
    return true;
}

// ============================================================================
// visitFalseLiteral — литерал FALSE.
// Возвращаем false.
// ============================================================================
std::any LogicInterpreter::visitFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) {
    return false;
}

// ============================================================================
// visitParens — выражение в скобках.
// baseExpr : '(' orExpr ')'
// Просто передаём значение внутреннего выражения.
// ============================================================================
std::any LogicInterpreter::visitParens(LogicGrammarParser::ParensContext* ctx) {
    return visit(ctx->orExpr());
}
