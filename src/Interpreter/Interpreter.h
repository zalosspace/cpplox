#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector> 

#include "../Lox/Expr.h"
#include "../Lox/Stmt.h"

class Interpreter : public Expr::Visitor, Stmt::Visitor {
public:
    // Override
    Value visitBinaryExpr(const Binary& expr) override;
    Value visitGroupingExpr(const Grouping& expr) override;
    Value visitLiteralExpr(const Literal& expr) override;
    Value visitUnaryExpr(const Unary& expr) override;

    // Public API
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

private:
    Value evaluate(const Expr& expr); 
    void execute(const Stmt& stmt);
    bool isTruthy(const Value& value);
    bool isEqual(const Value& a, const Value& b);
    std::string stringify(const Value& value);
    void checkNumberOperand(Token operator_, Value operand);
    void checkNumberOperands(Token operator_, Value left, Value right);

    // Override
    Value visitExpressionStmt(const Stmt::Expression& stmt) override;
    Value visitPrintStmt(const Stmt::Print& stmt) override;

};

#endif
