#include "AstPrinter.h"
#include <functional>

std::string AstPrinter::print(const Expr& expr) {
    return std::get<std::string>(expr.accept(*this));
}

std::string AstPrinter::print(const Stmt& stmt) {
    Value result = stmt.accept(*this);
    if (auto s = std::get_if<std::string>(&result)) return *s;
    return ""; 
}

void AstPrinter::print(const std::vector<std::unique_ptr<Stmt>>& statements) {
    for (const auto& stmt : statements) {
        std::cout << print(*stmt) << "\n";
    }
}

// ---------- Expr Visitor Implementation ---------- 
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

// ---------- Stmt Visitor Implementation ---------- 
Value AstPrinter::visitExpressionStmt(const Stmt::Expression& stmt) {
    return print(*stmt.expression);
}

// Private Implementation 
std::string AstPrinter::parenthesize(const std::string& name, 
                                     const std::vector<std::reference_wrapper<const Expr>>& exprs) {
    std::string builder = "(" + name;
    for(const Expr& expr: exprs) {
        builder += " ";
        builder += print(expr);
    }
    builder += ")";
    return builder;
}
