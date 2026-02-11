#pragma once

#include "../Token/Token.h"
#include "../Runtime/Value.h"

#include <variant>
#include <memory>
#include <string>
#include <any>
#include <vector>

class Expr {
public:
    // Nested Class
    class Binary;
    class Grouping;
    class Literal;
    class Unary;
    class Variable;
    class Assign;
    class Logical;
    class Call;

    // Visitor Function 
    struct Visitor {
        virtual Value visitBinaryExpr(const Binary& expr) { return {}; };
        virtual Value visitGroupingExpr(const Grouping& expr) { return {}; };
        virtual Value visitLiteralExpr(const Literal& expr) { return {}; };
        virtual Value visitUnaryExpr(const Unary& expr) { return {}; };
        virtual Value visitVarExpr(const Variable& expr) { return {}; };
        virtual Value visitAssignExpr(const Assign& expr) { return {}; };
        virtual Value visitLogicalExpr(const Logical& expr) { return {}; };
        virtual Value visitCallExpr(const Call& expr) { return {}; };

        virtual ~Visitor() = default;
    };

    virtual ~Expr() = default;

    virtual Value accept(Visitor& visitor) const = 0;
};

// ------------------ AST Nodes ------------------

class Expr::Binary : public Expr {
public:
    Binary(std::unique_ptr<Expr> left, Token operator_, std::unique_ptr<Expr> right)
        : left(std::move(left)), operator_(operator_), right(std::move(right)) {}

    std::unique_ptr<Expr> left;
    Token operator_;
    std::unique_ptr<Expr> right;

    // Override
    Value accept(Visitor& visitor) const override {
        return visitor.visitBinaryExpr(*this);
    }
};

class Expr::Grouping : public Expr {
public:
    explicit Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::unique_ptr<Expr> expression;

    // Override
    Value accept(Visitor& visitor) const override {
        return visitor.visitGroupingExpr(*this);
    }
};

class Expr::Literal : public Expr {
public:
    explicit Literal(Value value)
        : value(value) {}

    Value value;

    // stringify
    std::string toString() const {
        if (std::holds_alternative<std::monostate>(value)) 
            return "nil";

        if (auto d = std::get_if<double>(&value))
            return std::to_string(*d);

        if (auto s = std::get_if<std::string>(&value))
            return *s;

        if (auto b = std::get_if<bool>(&value))
            return *b ? "true" : "false";

        return "nil";
    }

    // Override
    Value accept(Visitor& visitor) const override {
        return visitor.visitLiteralExpr(*this);
    }
};

class Expr::Unary : public Expr {
public:
    Token operator_;
    std::unique_ptr<Expr> right;

    Unary(Token operator_, std::unique_ptr<Expr> right)
        : operator_(operator_), right(std::move(right)) {}

    // Override
    Value accept(Visitor& visitor) const override {
        return visitor.visitUnaryExpr(*this);
    }
};

class Expr::Variable : public Expr {
public:
    Token name;

    Variable(Token name)
        : name(name) {}

    // Override
    Value accept(Visitor& visitor) const override {
        return visitor.visitVarExpr(*this);
    }
};

class Expr::Assign : public Expr {
public:
    Token name;
    std::unique_ptr<Expr> value;

    Assign(Token name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}


    // Override
    Value accept(Visitor& visitor) const override {
        return visitor.visitAssignExpr(*this);
    }
};

class Expr::Logical: public Expr {
public:
    std::unique_ptr<Expr> left;
    Token operator_;
    std::unique_ptr<Expr> right;

    Logical(std::unique_ptr<Expr> left, 
            Token operator_,
            std::unique_ptr<Expr> right)
        :left(std::move(left)),
        operator_(std::move(operator_)),
        right(std::move(right)) {}

    // Override
    Value accept(Visitor& visitor) const override {
        return visitor.visitLogicalExpr(*this);
    }
};

class Expr::Call: public Expr {
public:
    std::unique_ptr<Expr> callee;
    Token paren;
    std::vector<std::unique_ptr<Expr>> arguments;

    Call(std::unique_ptr<Expr> callee, 
            Token paren,
            std::vector<std::unique_ptr<Expr>> arguments)
        :callee(std::move(callee)),
        paren(std::move(paren)),
        arguments(std::move(arguments)) {}

    // Override
    Value accept(Visitor& visitor) const override {
        return visitor.visitCallExpr(*this);
    }
};
