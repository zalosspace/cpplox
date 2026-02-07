#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector> 

#include "../Lox/Expr.h"
#include "../Lox/Stmt.h"
#include "../Semantic/Environment.h"

class Interpreter : public Expr::Visitor, Stmt::Visitor {
public:
    Interpreter() 
        : globalEnvironment(std::make_unique<Environment>()),
          environment(globalEnvironment.get()) {}

    // Override
    Value visitBinaryExpr(const Binary& expr) override;
    Value visitGroupingExpr(const Grouping& expr) override;
    Value visitLiteralExpr(const Literal& expr) override;
    Value visitUnaryExpr(const Unary& expr) override;

    // Public API
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

private:
    std::unique_ptr<Environment> globalEnvironment;
    Environment* environment;

    Value evaluate(const Expr& expr); 
    void execute(const Stmt& stmt);
    void executeBlock(
        const std::vector<std::unique_ptr<Stmt>>& statements,
        Environment& environment);
    bool isTruthy(const Value& value);
    bool isEqual(const Value& a, const Value& b);
    std::string stringify(const Value& value);
    void checkNumberOperand(Token operator_, Value operand);
    void checkNumberOperands(Token operator_, Value left, Value right);

    // Override
    Value visitExpressionStmt(const Stmt::Expression& stmt) override;
    Value visitVarExpr(const Variable& expr) override;
    Value visitVarStmt(const Stmt::Var& stmt) override;
    Value visitPrintStmt(const Stmt::Print& stmt) override;
    Value visitAssignExpr(const Assign& expr) override;
    Value visitBlockStmt(const Stmt::Block& stmt) override;

};

#endif
