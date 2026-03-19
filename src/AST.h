#pragma once
#include <string>
#include <memory>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::string toString() const = 0;
    virtual bool evaluate() const = 0;
};

class ConstNode : public ASTNode {
private:
    bool value;
public:
    ConstNode(bool val) : value(val) {}
    std::string toString() const override;
    bool evaluate() const override { return value; }
};

class NotNode : public ASTNode {
private:
    std::shared_ptr<ASTNode> child;
public:
    NotNode(std::shared_ptr<ASTNode> node) : child(node) {}
    std::string toString() const override;
    bool evaluate() const override { return !child->evaluate(); }
};

class AndNode : public ASTNode {
private:
    std::vector<std::shared_ptr<ASTNode>> children;
public:
    AndNode() = default;
    void addChild(std::shared_ptr<ASTNode> node) { children.push_back(node); }
    std::string toString() const override;
    bool evaluate() const override;
};

class OrNode : public ASTNode {
private:
    std::vector<std::shared_ptr<ASTNode>> children;
public:
    OrNode() = default;
    void addChild(std::shared_ptr<ASTNode> node) { children.push_back(node); }
    std::string toString() const override;
    bool evaluate() const override;
};
