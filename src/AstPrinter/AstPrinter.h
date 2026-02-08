#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "../Lox/Expr.h"

class AstPrinter: public Expr::Visitor{
    public:
        std::string print(const Expr& expr);
        Value visitBinaryExpr(const Expr::Binary& expr) override;
        Value visitGroupingExpr(const Expr::Grouping& expr) override;
        Value visitLiteralExpr(const Expr::Literal& expr) override;
        Value visitUnaryExpr(const Expr::Unary& expr) override;

    private:
        std::string parenthesize(const std::string& name, const std::vector<std::reference_wrapper<const Expr>>& exprs);

};
