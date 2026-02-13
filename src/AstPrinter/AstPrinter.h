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

    Value visitExpressionStmt(const Stmt::Expression& stmt) override;

private:
    std::string parenthesize(const std::string& name, const std::vector<std::reference_wrapper<const Expr>>& exprs);

};
