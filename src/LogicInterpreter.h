#pragma once
// ============================================================================
// LogicInterpreter.h — Visitor-интерпретатор для логических выражений.
//
// Наследуется от сгенерированного LogicGrammarBaseVisitor.
// Каждый visit-метод соответствует метке (#) в грамматике.
// Выражения возвращают std::shared_ptr<ASTNode> через std::any.
//
// Конвейер: текст → лексер → токены → парсер → CST → Visitor (этот файл)
// ============================================================================

#include <any>
#include <memory>
#include <vector>
#include "LogicGrammarBaseVisitor.h"
#include "AST.h"

/**
 * Visitor-интерпретатор для логических выражений.
 * Наследуется от сгенерированного LogicGrammarBaseVisitor
 * и переопределяет visit-методы для построения AST и вычисления.
 *
 * Это рекомендуемый подход для серьёзных проектов:
 * - Грамматика остаётся чистым описанием языка
 * - Код модульный и тестируемый
 */
class LogicInterpreter : public LogicGrammarBaseVisitor {
public:
    /**
     * Посещение правила orExpr (дизъюнкция ||)
     * Возвращает std::any, содержащий std::shared_ptr<ASTNode>
     */
    std::any visitOrOp(LogicGrammarParser::OrOpContext* ctx) override;

    /**
     * Посещение правила andExpr (конъюнкция &&)
     */
    std::any visitAndOp(LogicGrammarParser::AndOpContext* ctx) override;

    /**
     * Посещение правила notExpr (отрицание !)
     */
    std::any visitNotOp(LogicGrammarParser::NotOpContext* ctx) override;

    /**
     * Посещение правила baseExpr (TRUE, FALSE, скобки)
     */
    std::any visitNotBase(LogicGrammarParser::NotBaseContext* ctx) override;

    /**
     * Посещение литерала TRUE
     */
    std::any visitTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) override;

    /**
     * Посещение литерала FALSE
     */
    std::any visitFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) override;

    /**
     * Посещение правила для скобок
     */
    std::any visitParens(LogicGrammarParser::ParensContext* ctx) override;
};
