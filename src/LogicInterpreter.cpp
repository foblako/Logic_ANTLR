#include "LogicInterpreter.h"

std::any LogicInterpreter::visitOrOp(LogicGrammarParser::OrOpContext* ctx) {
    std::vector<std::shared_ptr<ASTNode>> children;

    auto result = visitAndOp(ctx->andExpr(0));
    children.push_back(std::any_cast<std::shared_ptr<ASTNode>>(result));

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

std::any LogicInterpreter::visitAndOp(LogicGrammarParser::AndOpContext* ctx) {
    std::vector<std::shared_ptr<ASTNode>> children;

    auto result = visitNotOp(ctx->notExpr(0));
    children.push_back(std::any_cast<std::shared_ptr<ASTNode>>(result));

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

std::any LogicInterpreter::visitNotOp(LogicGrammarParser::NotOpContext* ctx) {
    auto result = visitNotOp(ctx->notExpr());
    auto child = std::any_cast<std::shared_ptr<ASTNode>>(result);
    auto notNode = std::make_shared<NotNode>(child);
    return notNode;
}

std::any LogicInterpreter::visitNotBase(LogicGrammarParser::NotBaseContext* ctx) {
    return visitBaseExpr(ctx->baseExpr());
}

std::any LogicInterpreter::visitTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) {
    return std::make_shared<ConstNode>(true);
}

std::any LogicInterpreter::visitFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) {
    return std::make_shared<ConstNode>(false);
}

std::any LogicInterpreter::visitParens(LogicGrammarParser::ParensContext* ctx) {
    return visitOrOp(ctx->orExpr());
}
