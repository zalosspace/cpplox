#pragma once

#include "../Token/Token.h"
#include "../Token/Token.h"
#include "../Runtime/Value.h"
#include "../Runtime/Expr.h"

#include <utility>
#include <variant>
#include <memory>
#include <string>
#include <vector>

class Stmt {
public:
    // Nested Class
    class Expression;
    class Print;
    class Var;
    class Block;
    class If;
    class While;
    class Function;
    class Return;

    // Visitor Function 
    struct Visitor {
        virtual Value visitExpressionStmt(const Expression& stmt) = 0;
        virtual Value visitPrintStmt(const Print& stmt) = 0;
        virtual Value visitVarStmt(const Var& stmt) = 0;
        virtual Value visitBlockStmt(const Block& stmt) = 0;
        virtual Value visitIfStmt(const If& stmt) = 0;
        virtual Value visitWhileStmt(const While& stmt) = 0;
        virtual Value visitFunctionStmt(const Function& stmt) = 0;
        virtual Value visitReturnStmt(const Return& stmt) = 0;

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

class Stmt::If : public Stmt {
public:
    If(std::unique_ptr<Expr> condition,
          std::unique_ptr<Stmt> thenBranch,
          std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)),
        thenBranch(std::move(thenBranch)),
        elseBranch(std::move(elseBranch)) {}

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    Value accept(Visitor& visitor) const override {
        return visitor.visitIfStmt(*this);
    }
};

class Stmt::While : public Stmt {
public:
    While(std::unique_ptr<Expr> condition,
          std::unique_ptr<Stmt> body)
        : condition(std::move(condition)),
        body(std::move(body)) {}

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    Value accept(Visitor& visitor) const override {
        return visitor.visitWhileStmt(*this);
    }
};

class Stmt::Function : public Stmt {
public:
    Token name;
    std::vector<Token> params;
    std::vector<std::unique_ptr<Stmt>> body;

    Function(Token name,
             std::vector<Token> params,
             std::vector<std::unique_ptr<Stmt>> body)
      : name(name), params(std::move(params)), body(std::move(body)) {}

    Value accept(Visitor& visitor) const override {
        return visitor.visitFunctionStmt(*this);
    }
};

class Stmt::Return : public Stmt {
public:
    Token keyword;
    std::unique_ptr<Expr> value;

    Return(Token keyword, std::unique_ptr<Expr> value)
      : keyword(keyword), value(std::move(value)) {}

    Value accept(Visitor& visitor) const override {
        return visitor.visitReturnStmt(*this);
    }
};
