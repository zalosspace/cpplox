#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "../Lox/Expr.h"

class AstPrinter: public Expr::Visitor{
    public:
        std::string print(const Expr& expr);
        Value visitBinaryExpr(const Binary& expr) override;
        Value visitGroupingExpr(const Grouping& expr) override;
        Value visitLiteralExpr(const Literal& expr) override;
        Value visitUnaryExpr(const Unary& expr) override;

    private:
        std::string parenthesize(const std::string& name, const std::vector<std::reference_wrapper<const Expr>>& exprs);

};
