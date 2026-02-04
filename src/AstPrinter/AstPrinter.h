#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "../Lox/Expr.h"

class AstPrinter: public Expr::Visitor{
    public:
        std::string print(const Expr& expr);
        std::any visitBinaryExpr(const Binary& expr) override;
        std::any visitGroupingExpr(const Grouping& expr) override;
        std::any visitLiteralExpr(const Literal& expr) override;
        std::any visitUnaryExpr(const Unary& expr) override;

    private:
        std::string parenthesize(const std::string& name, const std::vector<std::reference_wrapper<const Expr>>& exprs);

};
