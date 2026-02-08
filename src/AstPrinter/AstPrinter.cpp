#include "AstPrinter.h"
#include <any>
#include <functional>

std::string AstPrinter::print(const Expr& expr) {
    return std::any_cast<std::string>(expr.accept(*this));
}

// Visitor Implementation
Value AstPrinter::visitBinaryExpr(const Expr::Binary& expr) {
    return parenthesize(expr.operator_.lexeme, {*expr.left, *expr.right});
}

Value AstPrinter::visitGroupingExpr(const Expr::Grouping& expr) {
    return parenthesize("group", {*expr.expression});
}

Value AstPrinter::visitLiteralExpr(const Expr::Literal& expr) {
    return expr.toString();
}

Value AstPrinter::visitUnaryExpr(const Expr::Unary& expr) {
    return parenthesize(expr.operator_.lexeme, {*expr.right});
}

// Private Implementation 
std::string AstPrinter::parenthesize(const std::string& name, 
                                     const std::vector<std::reference_wrapper<const Expr>>& exprs) {
    std::string bulider = "(" + name;
    for(const Expr& expr: exprs) {
        bulider += " ";
        bulider += print(expr);
    }
    bulider += ")";
    return bulider;
}
