#pragma once

#include "../Token/Token.h"
#include "Expr.h"
#include <utility>
#include <variant>
#include <memory>
#include <string>
#include <vector>

using Value = std::variant<std::monostate, double, std::string, bool>;

class Stmt {
    public:
        class Expression;
        class Print;
        class Var;
        class Block;

        struct Visitor {
            virtual Value visitExpressionStmt(const Expression& expr) = 0;
            virtual Value visitPrintStmt(const Print& expr) = 0;
            virtual Value visitVarStmt(const Var& stmt) = 0;
            virtual Value visitBlockStmt(const Block& stmt) = 0;
            virtual ~Visitor() = default;
        };

        virtual ~Stmt() = default;
        virtual Value accept(Visitor& visitor) const = 0;
};

// Nested Class
class Stmt::Expression : public Stmt {
    public:
        Expression(std::unique_ptr<Expr> expression)
            : expression(std::move(expression)) {}

        std::unique_ptr<Expr> expression;

        Value accept(Visitor& visitor) const override {
            return visitor.visitExpressionStmt(*this);
        }
};

class Stmt::Print : public Stmt {
public:
    Print(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::unique_ptr<Expr> expression;

    Value accept(Visitor& visitor) const override {
        return visitor.visitPrintStmt(*this);
    }
};

class Stmt::Var : public Stmt {
public:
    Var(Token name, std::unique_ptr<Expr> initializer)
        : name(name), initializer(std::move(initializer)) {}

    Token name;
    std::unique_ptr<Expr> initializer;

    Value accept(Visitor& visitor) const override {
        return visitor.visitVarStmt(*this);
    }
};

class Stmt::Block : public Stmt {
public:
    Block(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}

    std::vector<std::unique_ptr<Stmt>> statements;

    Value accept(Visitor& visitor) const override {
        return visitor.visitBlockStmt(*this);
    }
};
