#include "AstPrinter.h"
#include <functional>

std::string AstPrinter::print(const Expr& expr) {
    Value result = expr.accept(*this);
    if (auto s = std::get_if<std::string>(&result)) return *s;
    return "(unimplemented expr)"; 
}

std::string AstPrinter::print(const Stmt& stmt) {
    Value result = stmt.accept(*this);
    if (auto s = std::get_if<std::string>(&result)) return *s;
    return "(unimplemented stmt)"; 
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

Value AstPrinter::visitVarExpr(const Expr::Variable& expr) {
    return expr.name.lexeme;
}

Value AstPrinter::visitCallExpr(const Expr::Call& expr) {
    std::string result = "(call " + print(*expr.callee);
    for (const auto& arg : expr.arguments)
        result += " " + print(*arg);
    result += ")";
    return result;
}

// ---------- Stmt Visitor Implementation ---------- 
Value AstPrinter::visitExpressionStmt(const Stmt::Expression& stmt) {
    return print(*stmt.expression);
}

Value AstPrinter::visitPrintStmt(const Stmt::Print& stmt) {
    return parenthesize("print", {*stmt.expression});
}

Value AstPrinter::visitVarStmt(const Stmt::Var& stmt) {
    if (stmt.initializer)
        return parenthesize("var " + stmt.name.lexeme, {*stmt.initializer});
    return std::string("(var " + stmt.name.lexeme + ")");
}

Value AstPrinter::visitBlockStmt(const Stmt::Block& stmt) {
    std::string result = "(block";
    for (const auto& s : stmt.statements)
        result += " " + print(*s);
    result += ")";
    return result;
}

Value AstPrinter::visitIfStmt(const Stmt::If& stmt) {
    std::string result = "(if " + print(*stmt.condition);
    result += " " + print(*stmt.thenBranch);
    if (stmt.elseBranch)
        result += " " + print(*stmt.elseBranch);
    result += ")";
    return result;
}

Value AstPrinter::visitWhileStmt(const Stmt::While& stmt) {
    return std::string("(while " + print(*stmt.condition) + " " + print(*stmt.body) + ")");
}

Value AstPrinter::visitFunctionStmt(const Stmt::Function& stmt) {
    std::string result = "(fun " + stmt.name.lexeme + "(";
    for (size_t i = 0; i < stmt.params.size(); i++) {
        if (i > 0) result += " ";
        result += stmt.params[i].lexeme;
    }
    result += ") (block";
    for (const auto& s : stmt.body)
        result += " " + print(*s);
    result += "))";
    return result;
}

Value AstPrinter::visitReturnStmt(const Stmt::Return& stmt) {
    if (stmt.value)
        return parenthesize("return", {*stmt.value});
    return std::string("(return)");
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
