#include "AST.h"
#include <sstream>

// ConstNode
std::string ConstNode::toString() const {
    return value ? "TRUE" : "FALSE";
}

// NotNode
std::string NotNode::toString() const {
    return "!" + child->toString();
}

// AndNode
std::string AndNode::toString() const {
    if (children.empty()) return "";
    std::ostringstream oss;
    oss << "(" << children[0]->toString();
    for (size_t i = 1; i < children.size(); ++i) {
        oss << " && " << children[i]->toString();
    }
    oss << ")";
    return oss.str();
}

bool AndNode::evaluate() const {
    for (const auto& child : children) {
        if (!child->evaluate()) return false;
    }
    return true;
}

// OrNode
std::string OrNode::toString() const {
    if (children.empty()) return "";
    std::ostringstream oss;
    oss << "(" << children[0]->toString();
    for (size_t i = 1; i < children.size(); ++i) {
        oss << " || " << children[i]->toString();
    }
    oss << ")";
    return oss.str();
}

bool OrNode::evaluate() const {
    for (const auto& child : children) {
        if (child->evaluate()) return true;
    }
    return false;
}
