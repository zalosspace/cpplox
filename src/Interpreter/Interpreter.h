#pragma once

#include "../Runtime/Expr.h"
#include "../Runtime/Stmt.h"
#include "../Semantic/Environment.h"

#include <vector> 

class Interpreter : public Expr::Visitor, Stmt::Visitor {
public:
    Interpreter() 
        : globalEnvironment(std::make_unique<Environment>()),
          environment(globalEnvironment.get()) {}

    // Override
    Value visitBinaryExpr(const Expr::Binary& expr) override;
    Value visitGroupingExpr(const Expr::Grouping& expr) override;
    Value visitLiteralExpr(const Expr::Literal& expr) override;
    Value visitLogicalExpr(const Expr::Logical &expr) override;
    Value visitUnaryExpr(const Expr::Unary& expr) override;

    // Public API
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

private:
    std::unique_ptr<Environment> globalEnvironment;
    Environment* environment;

    Value evaluate(const Expr& expr); 
    void execute(const Stmt& stmt);
    void executeBlock(
        const std::vector<std::unique_ptr<Stmt>>& statements,
        Environment& environment);
    bool isTruthy(const Value& value);
    bool isEqual(const Value& a, const Value& b);
    std::string stringify(const Value& value);
    void checkNumberOperand(Token operator_, Value operand);
    void checkNumberOperands(Token operator_, Value left, Value right);

    // Override
    Value visitVarExpr(const Expr::Variable& expr) override;
    Value visitAssignExpr(const Expr::Assign& expr) override;

    Value visitExpressionStmt(const Stmt::Expression& stmt) override;
    Value visitVarStmt(const Stmt::Var& stmt) override;
    Value visitWhileStmt(const Stmt::While& stmt) override;
    Value visitPrintStmt(const Stmt::Print& stmt) override;
    Value visitBlockStmt(const Stmt::Block& stmt) override;
    Value visitIfStmt(const Stmt::If& stmt) override;
};
