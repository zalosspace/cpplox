#pragma once

#include "../Token/Token.h"
#include <variant>
#include <string>

class Binary;
class Grouping;
class Literal;
class Unary;

class Expr {
public:
    // Visitor pattern 
    struct Visitor {
        virtual std::string visitBinaryExpr(const Binary* expr) = 0;
        virtual std::string visitGroupingExpr(const Grouping* expr) = 0;
        virtual std::string visitLiteralExpr(const Literal* expr) = 0;
        virtual std::string visitUnaryExpr(const Unary* expr) = 0;

        virtual ~Visitor() = default;
    };

    virtual ~Expr() = default;

    virtual std::string accept(Visitor& visitor) const = 0;
};

// ------------------ AST Nodes ------------------

class Binary : public Expr {
public:
    Binary(Expr* left, Token operator_, Expr* right)
        : left(left), operator_(operator_), right(right) {}

    Expr* left;
    Token operator_;
    Expr* right;

    std::string accept(Visitor& visitor) const override {
        return visitor.visitBinaryExpr(this);
    }
};

class Grouping : public Expr {
public:
    explicit Grouping(Expr* expression)
        : expression(expression) {}

    Expr* expression;

    std::string accept(Visitor& visitor) const override {
        return visitor.visitGroupingExpr(this);
    }
};

class Literal : public Expr {
public:
    explicit Literal(std::any value)
        : value(value) {}

    std::any value;

    // stringify
    std::string toString() const {
        if (!value.has_value()) return "nil";

        if (value.type() == typeid(double))
            return std::to_string(std::any_cast<double>(value));

        if (value.type() == typeid(std::string))
            return std::any_cast<std::string>(value);

        if (value.type() == typeid(bool))
            return std::any_cast<bool>(value) ? "true" : "false";

        return "nil";
    }

    std::string accept(Visitor& visitor) const override {
        return visitor.visitLiteralExpr(this);
    }
};

class Unary : public Expr {
public:
    Unary(Token operator_, Expr* right)
        : operator_(operator_), right(right) {}

    Token operator_;
    Expr* right;

    std::string accept(Visitor& visitor) const override {
        return visitor.visitUnaryExpr(this);
    }
};

