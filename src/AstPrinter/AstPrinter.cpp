#include "AstPrinter.h"

std::string AstPrinter::print(const Expr& expr) {
    return expr.accept(*this);
}

// Visitor Implementation
std::string AstPrinter::visitBinaryExpr(const Binary* expr) {
    return parenthesize(expr->operator_.lexeme, {expr->left, expr->right});
}

std::string AstPrinter::visitGroupingExpr(const Grouping* expr) {
    return parenthesize("group", {expr->expression});
}

std::string AstPrinter::visitLiteralExpr(const Literal* expr) {
    return expr->toString();
}

std::string AstPrinter::visitUnaryExpr(const Unary* expr) {
    return parenthesize(expr->operator_.lexeme, {expr->right});
}

// Private Implementation 
std::string AstPrinter::parenthesize(const std::string& name, const std::vector<const Expr*>& exprs) {
    std::string result = "(" + name;
    for(const Expr* expr: exprs) {
        result += " ";
        result += expr->accept(*this);
    }
    result += ")";
    return result;
}

