#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "../Runtime/Expr.h"
#include "../Runtime/Stmt.h"

class AstPrinter: public Expr::Visitor, Stmt::Visitor {
public:
    std::string print(const Stmt& stmt);
    std::string print(const Expr& expr);
    void print(const std::vector<std::unique_ptr<Stmt>>& statements);

    Value visitBinaryExpr(const Expr::Binary& expr) override;
    Value visitGroupingExpr(const Expr::Grouping& expr) override;
    Value visitLiteralExpr(const Expr::Literal& expr) override;
    Value visitUnaryExpr(const Expr::Unary& expr) override;
    Value visitVarExpr(const Expr::Variable& expr) override;
    Value visitCallExpr(const Expr::Call& expr) override;

    Value visitExpressionStmt(const Stmt::Expression& stmt) override;
    Value visitPrintStmt(const Stmt::Print& stmt) override;
    Value visitVarStmt(const Stmt::Var& stmt) override;
    Value visitBlockStmt(const Stmt::Block& stmt) override;
    Value visitIfStmt(const Stmt::If& stmt) override;
    Value visitWhileStmt(const Stmt::While& stmt) override;
    Value visitFunctionStmt(const Stmt::Function& stmt) override;
    Value visitReturnStmt(const Stmt::Return& stmt) override;

private:
    std::string parenthesize(const std::string& name, const std::vector<std::reference_wrapper<const Expr>>& exprs);

};
