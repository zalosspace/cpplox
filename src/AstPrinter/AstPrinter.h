#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "../Lox/Expr.h"

class AstPrinter: public Expr::Visitor{
    public:
        std::string print(const Expr& expr);
        std::string visitBinaryExpr(const Binary* expr);
        std::string visitGroupingExpr(const Grouping* expr);
        std::string visitLiteralExpr(const Literal* expr);
        std::string visitUnaryExpr(const Unary* expr);

    private:
        std::string parenthesize(const std::string& name, const std::vector<const Expr*>& exprs);

};
