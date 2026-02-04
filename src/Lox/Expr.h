#pragma once

#include "../Token/Token.h"
#include <variant>
#include <memory>
#include <string>
#include <any>

class Binary;
class Grouping;
class Literal;
class Unary;

class Expr {
public:
    // Visitor pattern 
    struct Visitor {
        virtual std::any visitBinaryExpr(const Binary& expr) = 0;
        virtual std::any visitGroupingExpr(const Grouping& expr) = 0;
        virtual std::any visitLiteralExpr(const Literal& expr) = 0;
        virtual std::any visitUnaryExpr(const Unary& expr) = 0;

        virtual ~Visitor() = default;
    };

    virtual ~Expr() = default;

    virtual std::any accept(Visitor& visitor) const = 0;
};

// ------------------ AST Nodes ------------------

class Binary : public Expr {
public:
    Binary(std::unique_ptr<Expr> left, Token operator_, std::unique_ptr<Expr> right)
        : left(std::move(left)), operator_(operator_), right(std::move(right)) {}

    std::unique_ptr<Expr> left;
    Token operator_;
    std::unique_ptr<Expr> right;

    // Override
    std::any accept(Visitor& visitor) const override {
        return visitor.visitBinaryExpr(*this);
    }
};

class Grouping : public Expr {
public:
    explicit Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::unique_ptr<Expr> expression;

    // Override
    std::any accept(Visitor& visitor) const override {
        return visitor.visitGroupingExpr(*this);
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

    // Override
    std::any accept(Visitor& visitor) const override {
        return visitor.visitLiteralExpr(*this);
    }
};

class Unary : public Expr {
public:
    Unary(Token operator_, std::unique_ptr<Expr> right)
        : operator_(operator_), right(std::move(right)) {}

    Token operator_;
    std::unique_ptr<Expr> right;

    // Override
    std::any accept(Visitor& visitor) const override {
        return visitor.visitUnaryExpr(*this);
    }
};

