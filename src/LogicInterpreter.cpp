// ============================================================================
// LogicInterpreter.cpp — реализация Visitor-интерпретатора.
//
// Каждый visit-метод строит узел AST для соответствующей конструкции.
// Возвращаемое значение: std::any со std::shared_ptr<ASTNode>.
// ============================================================================

#include "LogicInterpreter.h"

/**
 * Посещение правила orExpr (дизъюнкция ||)
 * Возвращает std::any, содержащий std::shared_ptr<ASTNode>
 */
std::any LogicInterpreter::visitOrOp(LogicGrammarParser::OrOpContext* ctx) {
    std::vector<std::shared_ptr<ASTNode>> children;

    // Первый операнд
    auto result = visitAndOp(ctx->andExpr(0));
    children.push_back(std::any_cast<std::shared_ptr<ASTNode>>(result));

    // Остальные операнды (если есть операторы ||)
    for (size_t i = 1; i < ctx->andExpr().size(); ++i) {
        auto result = visitAndOp(ctx->andExpr(i));
        children.push_back(std::any_cast<std::shared_ptr<ASTNode>>(result));
    }

    if (children.size() == 1) {
        return children[0];
    }

    auto orNode = std::make_shared<OrNode>();
    for (const auto& child : children) {
        orNode->addChild(child);
    }

    return orNode;
}

/**
 * Посещение правила andExpr (конъюнкция &&)
 */
std::any LogicInterpreter::visitAndOp(LogicGrammarParser::AndOpContext* ctx) {
    std::vector<std::shared_ptr<ASTNode>> children;

    // Первый операнд
    auto result = visitNotOp(ctx->notExpr(0));
    children.push_back(std::any_cast<std::shared_ptr<ASTNode>>(result));

    // Остальные операнды (если есть операторы &&)
    for (size_t i = 1; i < ctx->notExpr().size(); ++i) {
        auto result = visitNotOp(ctx->notExpr(i));
        children.push_back(std::any_cast<std::shared_ptr<ASTNode>>(result));
    }

    if (children.size() == 1) {
        return children[0];
    }

    auto andNode = std::make_shared<AndNode>();
    for (const auto& child : children) {
        andNode->addChild(child);
    }

    return andNode;
}

/**
 * Посещение правила notExpr (отрицание !)
 * Правоассоциативный оператор
 */
std::any LogicInterpreter::visitNotOp(LogicGrammarParser::NotOpContext* ctx) {
    auto result = visitNotOp(ctx->notExpr());
    auto child = std::any_cast<std::shared_ptr<ASTNode>>(result);
    auto notNode = std::make_shared<NotNode>(child);
    return notNode;
}

/**
 * Посещение правила baseExpr (TRUE, FALSE, скобки)
 */
std::any LogicInterpreter::visitNotBase(LogicGrammarParser::NotBaseContext* ctx) {
    return visitBaseExpr(ctx->baseExpr());
}

/**
 * Посещение литерала TRUE
 */
std::any LogicInterpreter::visitTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) {
    return std::make_shared<ConstNode>(true);
}

/**
 * Посещение литерала FALSE
 */
std::any LogicInterpreter::visitFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) {
    return std::make_shared<ConstNode>(false);
}

/**
 * Посещение правила для скобок
 */
std::any LogicInterpreter::visitParens(LogicGrammarParser::ParensContext* ctx) {
    return visitOrOp(ctx->orExpr());
}
