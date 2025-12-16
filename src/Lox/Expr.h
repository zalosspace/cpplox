#pragma once

#include "../Token/Token.h"
#include <variant>
#include <string>

using Object = std::variant<std::monostate, double, std::string, bool>;

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
    explicit Literal(Object value)
        : value(value) {}

    Object value;

    // stringify
    std::string toString() const {
        if (std::holds_alternative<std::monostate>(value)) return "nil";
        if (auto v = std::get_if<double>(&value)) return std::to_string(*v);
        if (auto v = std::get_if<std::string>(&value)) return *v;
        if (auto v = std::get_if<bool>(&value)) return *v ? "true" : "false";
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

