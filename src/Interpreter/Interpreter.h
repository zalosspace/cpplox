#pragma once

#include "RuntimeError.h"
#include "../Runtime/Expr.h"
#include "../Runtime/Stmt.h"
#include "../Runtime/Value.h"
#include "../Runtime/Return.h"
#include "../Semantic/Environment.h"
#include "../Include/ClockCallable.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector> 

class Interpreter : public Expr::Visitor, Stmt::Visitor {
public:
    std::shared_ptr<Environment> globals;
    std::shared_ptr<Environment> environment;
    std::unordered_map<const Expr*, int> locals;

    Interpreter() 
        : globals(std::make_shared<Environment>()),
          environment(globals) 
    {
        globals->define(
            "clock",
            Value(std::make_shared<ClockCallable>())
        );
    }

    // Override
    Value visitBinaryExpr(const Expr::Binary& expr) override;
    Value visitGroupingExpr(const Expr::Grouping& expr) override;
    Value visitLiteralExpr(const Expr::Literal& expr) override;
    Value visitLogicalExpr(const Expr::Logical &expr) override;
    Value visitSetExpr(const Expr::Set& expr) override;
    Value visitUnaryExpr(const Expr::Unary& expr) override;

    // Public API
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

    void executeBlock(
        const std::vector<std::unique_ptr<Stmt>>& statements,
        std::shared_ptr<Environment> newEnv);

    void resolve(const Expr& expr, int depth);

private:
    Value evaluate(const Expr& expr); 
    void execute(const Stmt& stmt);
    bool isTruthy(const Value& value);
    bool isEqual(const Value& a, const Value& b);
    std::string stringify(const Value& value);
    void checkNumberOperand(Token operator_, Value operand);
    void checkNumberOperands(Token operator_, Value left, Value right);
    Value lookUpVariable(const Token& name, const Expr::Variable& expr);

    // Override
    Value visitVarExpr(const Expr::Variable& expr) override;
    Value visitAssignExpr(const Expr::Assign& expr) override;
    Value visitCallExpr(const Expr::Call& expr) override;
    Value visitGetExpr(const Expr::Get& expr) override;

    Value visitExpressionStmt(const Stmt::Expression& stmt) override;
    Value visitFunctionStmt(const Stmt::Function& stmt) override;
    Value visitVarStmt(const Stmt::Var& stmt) override;
    Value visitWhileStmt(const Stmt::While& stmt) override;
    Value visitPrintStmt(const Stmt::Print& stmt) override;
    Value visitReturnStmt(const Stmt::Return& stmt) override;
    Value visitBlockStmt(const Stmt::Block& stmt) override;
    Value visitClassStmt(const Stmt::Class& stmt) override;
    Value visitIfStmt(const Stmt::If& stmt) override;
};
