#pragma once
#include <vector>
#include <iostream>
#include <string>

#include "AstPrinter.h"

std::string AstPrinter::print(Expr& expr) {
    return expr.accept(*this);
}

// Visitor Implementation
std::string AstPrinter::visitBinaryExpr(Binary* expr) {
    return parenthesize(expr->operator_.lexeme, {expr->left, expr->right});
}

std::string AstPrinter::visitGroupingExpr(Grouping* expr) {
    return parenthesize("group", {expr->expression});
}

std::string AstPrinter::visitLiteralExpr(Literal* expr) {
    return expr->toString();
}

std::string AstPrinter::visitUnaryExpr(Unary* expr) {
    return parenthesize(expr->operator_.lexeme, {expr->right});
}

// Private Impletation 
std::string AstPrinter::parenthesize(const std::string& name, const std::vector<Expr*>& exprs) {
    std::string result = "(" + name;
    for(const Expr* expr: exprs) {
        result += " ";
        result += expr->accept(*this);
    }
    result += ")";
    return result;
}
