#pragma once

#include "../Token/Token.h"
#include "Expr.h"
#include <variant>
#include <memory>
#include <string>

using Value = std::variant<std::monostate, double, std::string, bool>;

class Stmt {
    public:
        class Expression;
        class Print;

        struct Visitor {
            virtual Value visitExpressionStmt(const Expression& expr) = 0;
            virtual Value visitPrintStmt(const Print& expr) = 0;
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

