#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "./Expr.h"

class AstPrinter: public Expr::Visitor{
    public:
        std::string print(Expr& expr);
        std::string visitBinaryExpr(Binary* expr);
        std::string visitGroupingExpr(Grouping* expr);
        std::string visitLiteralExpr(Literal* expr);
        std::string visitUnaryExpr(Unary* expr);

    private:
        std::string parenthesize(const std::string& name, const std::vector<Expr*>& exprs);

};
