#pragma once

#include "../Runtime/Stmt.h"
#include "../Runtime/Expr.h"
#include "../Runtime/Runtime.h"
#include "../Interpreter/Interpreter.h"
#include <stack>
#include <unordered_map>

enum FunctionType {
    NONE,
    FUNCTION
};

class Resolver : public Expr::Visitor, public Stmt::Visitor {
public:
    Resolver(Interpreter& interpreter)
    : interpreter(interpreter) {}

    Value visitBlockStmt(const Stmt::Block& stmt) override;
    Value visitExpressionStmt(const Stmt::Expression& stmt) override;
    Value visitFunctionStmt(const Stmt::Function& stmt) override;
    Value visitIfStmt(const Stmt::If& stmt) override;
    Value visitPrintStmt(const Stmt::Print& stmt) override;
    Value visitReturnStmt(const Stmt::Return& stmt) override;
    Value visitVarStmt(const Stmt::Var& stmt) override;
    Value visitWhileStmt(const Stmt::While& stmt) override;

    Value visitAssignExpr(const Expr::Assign& expr) override;
    Value visitBinaryExpr(const Expr::Binary& expr) override;
    Value visitCallExpr(const Expr::Call& expr) override;
    Value visitGroupingExpr(const Expr::Grouping& expr) override;
    Value visitLiteralExpr(const Expr::Literal& expr) override;
    Value visitLogicalExpr(const Expr::Logical& expr) override;
    Value visitUnaryExpr(const Expr::Unary& expr) override;
    Value visitVarExpr(const Expr::Variable& expr) override;

    void resolve(const std::vector<std::unique_ptr<Stmt>>& statements);

private:
    // Keep track of variable initialization
    Interpreter& interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;
    FunctionType currentFunction = FunctionType::NONE;

    // Helpers
    void resolve(Stmt& stmt);
    void resolve(Expr& expr);

    void beginScope();
    void endScope();
    // Mark variable as declared but not initialized
    void declare(const Token& name);
    // Mark variable as initialized
    void define(const Token& name);
    void resolveLocal(const Expr& expr, const Token& name);
    void resolveFunction(const Stmt::Function& function);
    void resolveFunction(const Stmt::Function& function,
                         const FunctionType& type);
};
