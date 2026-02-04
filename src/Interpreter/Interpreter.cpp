#include "../Lox/Lox.h"
#include "../Lox/Expr.h"
#include <any>
#include <iostream>
#include <memory>
#include <string>
#include "Interpreter.h"
#include "RuntimeError.h"

// ---------- Public API ----------
void Interpreter::interpret(const Expr& expression) {
    try {
        std::any value = evaluate(expression);
        std::cout << stringify(value);
    } 
    catch (RuntimeError error) {
        Lox::runtimeError(error);
    }
}

// ---------- Override Visitor Function ----------
std::any Interpreter::visitLiteralExpr(const Literal& expr) {
    return expr.value;
}

std::any Interpreter::visitUnaryExpr(const Unary& expr) {
    std::any right = evaluate(*expr.right);

    switch (expr.operator_.type) {
        case MINUS: 
            {
                checkNumberOperand(expr.operator_, right);
                double value = std::any_cast<double>(right);
                return -value;
            }

        case BANG:
            {
                bool value = isTruthy(right);
                return !value;
            }
    }

    // Unreachable.
    return nullptr;
}

std::any Interpreter::visitGroupingExpr(const Grouping& expr) {
    return expr.expression->accept(*this);
}

std::any Interpreter::visitBinaryExpr(const Binary& expr) {
    std::any left = evaluate(*expr.left);
    std::any right = evaluate(*expr.right);

    switch (expr.operator_.type) {
        // Comparison Operator
        case GREATER: 
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);

        case GREATER_EQUAL: 
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);

        case LESS: 
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);

        case LESS_EQUAL: 
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);

        // Binary Operator
        case MINUS: 
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);

        case PLUS:
            {
                if (left.type() == typeid(double) && right.type() == typeid(double)) {
                    return std::any_cast<double>(left) + std::any_cast<double>(right);
                }

                if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                    return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
                }

                throw RuntimeError(expr.operator_,
                        "Operands must be two numbers or two strings.");
            }

        case SLASH:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);

        case STAR:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);

        // Equality Operator
        case BANG_EQUAL:
            return !isEqual(left, right);
        case EQUAL_EQUAL:
            return isEqual(left, right);
    }

    // Unreachable.
    return nullptr;
}

// ---------- Private Function ----------
// ---------- Helper Function ----------
std::any Interpreter::evaluate(const Expr& expr) {
    return expr.accept(*this);
}

bool Interpreter::isTruthy(const std::any& value) {
    if (!value.has_value()) return false;

    if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value);
    }

    return true;
}

bool Interpreter::isEqual(const std::any& a, const std::any& b) {
    if (!a.has_value() && !b.has_value()) return true;
    if (!a.has_value() || !b.has_value()) return false;
    if (a.type() != b.type()) return false;

    // compare by actual type
    if (a.type() == typeid(double))
        return std::any_cast<double>(a) == std::any_cast<double>(b);

    if (a.type() == typeid(bool))
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);

    if (a.type() == typeid(std::string))
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);

    // Unreachable.
    return true;
}

std::string Interpreter::stringify(const std::any& value) {
    if (!value.has_value()) return "nil";

    if (value.type() == typeid(double)) {
        double num = std::any_cast<double>(value);
        std::string text = std::to_string(num);

        if (text.size() >= 2 && text.substr(text.size() - 2) == ".0") {
            text = text.substr(0, text.size() - 2);
        }

        return text;
    }

    if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value) ? "true" : "false";
    }

    if (value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(value);
    }

    return "nil";
}

void Interpreter::checkNumberOperand(Token operator_, std::any operand) {
    if (operand.type() == typeid(double)) 
        return;

    throw RuntimeError(operator_, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token operator_, std::any left, std::any right) {
    if (left.type() == typeid(double) &&
            left.type() == typeid(double)
       )
    {
        return;
    }

    throw RuntimeError(operator_, "Operand must be a numbers.");
}
