#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../Lox/Expr.h"

class Interpreter : public Expr::Visitor {
public:
    // Override
    std::any visitBinaryExpr(const Binary& expr) override;
    std::any visitGroupingExpr(const Grouping& expr) override;
    std::any visitLiteralExpr(const Literal& expr) override;
    std::any visitUnaryExpr(const Unary& expr) override;

    // Public API
    void interpret(const Expr& expression);

private:
    std::any evaluate(const Expr& expr); 
    bool isTruthy(const std::any& value);
    bool isEqual(const std::any& a, const std::any& b);
    std::string stringify(const std::any& value);
    void checkNumberOperand(Token operator_, std::any operand);
    void checkNumberOperands(Token operator_, std::any left, std::any right);
};

#endif
