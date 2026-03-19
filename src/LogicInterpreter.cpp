#include "LogicInterpreter.h"

// Helper методы для работы с shared_ptr<ASTNode> напрямую
static std::shared_ptr<ASTNode> evalOrExpr(LogicGrammarParser::OrExprContext* ctx);
static std::shared_ptr<ASTNode> evalOrOp(LogicGrammarParser::OrOpContext* ctx);
static std::shared_ptr<ASTNode> evalAndExpr(LogicGrammarParser::AndExprContext* ctx);
static std::shared_ptr<ASTNode> evalAndOp(LogicGrammarParser::AndOpContext* ctx);
static std::shared_ptr<ASTNode> evalNotExpr(LogicGrammarParser::NotExprContext* ctx);
static std::shared_ptr<ASTNode> evalNotOp(LogicGrammarParser::NotOpContext* ctx);
static std::shared_ptr<ASTNode> evalNotBase(LogicGrammarParser::NotBaseContext* ctx);
static std::shared_ptr<ASTNode> evalBaseExpr(LogicGrammarParser::BaseExprContext* ctx);
static std::shared_ptr<ASTNode> evalTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx);
static std::shared_ptr<ASTNode> evalFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx);
static std::shared_ptr<ASTNode> evalParens(LogicGrammarParser::ParensContext* ctx);

std::any LogicInterpreter::visitProgram(LogicGrammarParser::ProgramContext* ctx) {
    if (ctx->children.empty()) return nullptr;
    auto* child = ctx->children[0];
    
    if (auto* orOp = dynamic_cast<LogicGrammarParser::OrOpContext*>(child)) {
        return evalOrOp(orOp);
    }
    if (auto* orExpr = dynamic_cast<LogicGrammarParser::OrExprContext*>(child)) {
        return evalOrExpr(orExpr);
    }
    return nullptr;
}

std::any LogicInterpreter::visitOrOp(LogicGrammarParser::OrOpContext* ctx) {
    return evalOrOp(ctx);
}

std::any LogicInterpreter::visitOrExpr(LogicGrammarParser::OrExprContext* ctx) {
    return evalOrExpr(ctx);
}

std::any LogicInterpreter::visitAndOp(LogicGrammarParser::AndOpContext* ctx) {
    return evalAndOp(ctx);
}

std::any LogicInterpreter::visitAndExpr(LogicGrammarParser::AndExprContext* ctx) {
    return evalAndExpr(ctx);
}

std::any LogicInterpreter::visitNotOp(LogicGrammarParser::NotOpContext* ctx) {
    return evalNotOp(ctx);
}

std::any LogicInterpreter::visitNotExpr(LogicGrammarParser::NotExprContext* ctx) {
    return evalNotExpr(ctx);
}

std::any LogicInterpreter::visitNotBase(LogicGrammarParser::NotBaseContext* ctx) {
    return evalNotBase(ctx);
}

std::any LogicInterpreter::visitBaseExpr(LogicGrammarParser::BaseExprContext* ctx) {
    return evalBaseExpr(ctx);
}

std::any LogicInterpreter::visitTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) {
    return evalTrueLiteral(ctx);
}

std::any LogicInterpreter::visitFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) {
    return evalFalseLiteral(ctx);
}

std::any LogicInterpreter::visitParens(LogicGrammarParser::ParensContext* ctx) {
    return evalParens(ctx);
}

// Реализация helper-методов

static std::shared_ptr<ASTNode> evalOrExpr(LogicGrammarParser::OrExprContext* ctx) {
    if (ctx->children.empty()) return nullptr;
    auto* child = ctx->children[0];
    
    if (auto* andOp = dynamic_cast<LogicGrammarParser::AndOpContext*>(child)) {
        return evalAndOp(andOp);
    }
    if (auto* andExpr = dynamic_cast<LogicGrammarParser::AndExprContext*>(child)) {
        return evalAndExpr(andExpr);
    }
    return nullptr;
}

static std::shared_ptr<ASTNode> evalOrOp(LogicGrammarParser::OrOpContext* ctx) {
    std::vector<std::shared_ptr<ASTNode>> children;

    for (auto* andExpr : ctx->andExpr()) {
        if (auto* andOp = dynamic_cast<LogicGrammarParser::AndOpContext*>(andExpr)) {
            children.push_back(evalAndOp(andOp));
        } else {
            children.push_back(evalAndExpr(andExpr));
        }
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

static std::shared_ptr<ASTNode> evalAndExpr(LogicGrammarParser::AndExprContext* ctx) {
    if (ctx->children.empty()) return nullptr;
    auto* child = ctx->children[0];
    
    if (auto* notOp = dynamic_cast<LogicGrammarParser::NotOpContext*>(child)) {
        return evalNotOp(notOp);
    }
    if (auto* notBase = dynamic_cast<LogicGrammarParser::NotBaseContext*>(child)) {
        return evalNotBase(notBase);
    }
    if (auto* notExpr = dynamic_cast<LogicGrammarParser::NotExprContext*>(child)) {
        return evalNotExpr(notExpr);
    }
    return nullptr;
}

static std::shared_ptr<ASTNode> evalAndOp(LogicGrammarParser::AndOpContext* ctx) {
    std::vector<std::shared_ptr<ASTNode>> children;

    for (auto* notExpr : ctx->notExpr()) {
        if (auto* notOp = dynamic_cast<LogicGrammarParser::NotOpContext*>(notExpr)) {
            children.push_back(evalNotOp(notOp));
        } else if (auto* notBase = dynamic_cast<LogicGrammarParser::NotBaseContext*>(notExpr)) {
            children.push_back(evalNotBase(notBase));
        } else {
            children.push_back(evalNotExpr(notExpr));
        }
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

static std::shared_ptr<ASTNode> evalNotExpr(LogicGrammarParser::NotExprContext* ctx) {
    if (ctx->children.empty()) return nullptr;
    auto* child = ctx->children[0];
    
    if (auto* baseExpr = dynamic_cast<LogicGrammarParser::BaseExprContext*>(child)) {
        return evalBaseExpr(baseExpr);
    }
    return nullptr;
}

static std::shared_ptr<ASTNode> evalNotOp(LogicGrammarParser::NotOpContext* ctx) {
    auto* notExpr = ctx->notExpr();
    std::shared_ptr<ASTNode> child;
    
    if (auto* op = dynamic_cast<LogicGrammarParser::NotOpContext*>(notExpr)) {
        child = evalNotOp(op);
    } else if (auto* base = dynamic_cast<LogicGrammarParser::NotBaseContext*>(notExpr)) {
        child = evalNotBase(base);
    } else {
        child = evalNotExpr(notExpr);
    }
    
    return std::make_shared<NotNode>(child);
}

static std::shared_ptr<ASTNode> evalNotBase(LogicGrammarParser::NotBaseContext* ctx) {
    auto* baseExpr = ctx->baseExpr();
    if (auto* trueLit = dynamic_cast<LogicGrammarParser::TrueLiteralContext*>(baseExpr)) {
        return evalTrueLiteral(trueLit);
    }
    if (auto* falseLit = dynamic_cast<LogicGrammarParser::FalseLiteralContext*>(baseExpr)) {
        return evalFalseLiteral(falseLit);
    }
    if (auto* parens = dynamic_cast<LogicGrammarParser::ParensContext*>(baseExpr)) {
        return evalParens(parens);
    }
    return nullptr;
}

static std::shared_ptr<ASTNode> evalBaseExpr(LogicGrammarParser::BaseExprContext* ctx) {
    if (ctx->children.empty()) return nullptr;
    auto* child = ctx->children[0];
    
    if (auto* trueLit = dynamic_cast<LogicGrammarParser::TrueLiteralContext*>(child)) {
        return evalTrueLiteral(trueLit);
    }
    if (auto* falseLit = dynamic_cast<LogicGrammarParser::FalseLiteralContext*>(child)) {
        return evalFalseLiteral(falseLit);
    }
    if (auto* parens = dynamic_cast<LogicGrammarParser::ParensContext*>(child)) {
        return evalParens(parens);
    }
    return nullptr;
}

static std::shared_ptr<ASTNode> evalTrueLiteral(LogicGrammarParser::TrueLiteralContext* ctx) {
    return std::make_shared<ConstNode>(true);
}

static std::shared_ptr<ASTNode> evalFalseLiteral(LogicGrammarParser::FalseLiteralContext* ctx) {
    return std::make_shared<ConstNode>(false);
}

static std::shared_ptr<ASTNode> evalParens(LogicGrammarParser::ParensContext* ctx) {
    auto* orExpr = ctx->orExpr();
    if (auto* orOp = dynamic_cast<LogicGrammarParser::OrOpContext*>(orExpr)) {
        return evalOrOp(orOp);
    }
    if (auto* orExprCtx = dynamic_cast<LogicGrammarParser::OrExprContext*>(orExpr)) {
        return evalOrExpr(orExprCtx);
    }
    return nullptr;
}
