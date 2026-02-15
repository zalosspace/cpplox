#include "Resolver.h"

#include <unordered_map>
#include <variant>
#include <vector>

// ----------- Visitor -----------
Value Resolver::visitBlockStmt(const Stmt::Block& stmt) {
    beginScope();
    resolve(stmt.statements);
    endScope();

    return std::monostate{};
}

Value Resolver::visitClassStmt(const Stmt::Class& stmt) {
    declare(stmt.name);
    define(stmt.name);

    for (auto& method: stmt.methods) {
        FunctionType declaration = FunctionType::METHOD;
        resolveFunction(*method, declaration);
    }

    return std::monostate{};
}

Value Resolver::visitExpressionStmt(const Stmt::Expression& stmt) {
    resolve(*stmt.expression);

    return std::monostate{};
}

Value Resolver::visitFunctionStmt(const Stmt::Function& stmt) {
    declare(stmt.name);
    define(stmt.name);

    resolveFunction(stmt, FunctionType::FUNCTION);

    return std::monostate{};
}

Value Resolver::visitIfStmt(const Stmt::If& stmt) {
    resolve(*stmt.condition);
    resolve(*stmt.thenBranch);

    if (stmt.elseBranch != nullptr) 
        resolve(*stmt.elseBranch);

    return std::monostate{};
}

Value Resolver::visitPrintStmt(const Stmt::Print& stmt) {
    resolve(*stmt.expression);

    return std::monostate{};
}

Value Resolver::visitReturnStmt(const Stmt::Return& stmt) {
    if (currentFunction == FunctionType::NONE) {
        Runtime::error(stmt.keyword, 
                       "Can't return from top-level");
    }

    if (stmt.value != nullptr) {
        resolve(*stmt.value);
    }

    return std::monostate{};
}

Value Resolver::visitVarStmt(const Stmt::Var& stmt) {
    declare(stmt.name);

    if (stmt.initializer != nullptr) {
        resolve(*stmt.initializer);
    }

    define(stmt.name);

    return std::monostate{};
}

Value Resolver::visitWhileStmt(const Stmt::While& stmt) {
    resolve(*stmt.condition);
    resolve(*stmt.body);

    return std::monostate{};
}

Value Resolver::visitAssignExpr(const Expr::Assign& expr) {
    resolve(*expr.value);
    resolveLocal(expr, expr.name);

    return std::monostate{};
}

Value Resolver::visitBinaryExpr(const Expr::Binary& expr) {
    resolve(*expr.left);
    resolve(*expr.right);

    return std::monostate{};
}

Value Resolver::visitCallExpr(const Expr::Call& expr) {
    resolve(*expr.callee);

    for (auto& argument: expr.arguments){
        resolve(*argument);
    }

    return std::monostate{};
}

Value Resolver::visitGetExpr(const Expr::Get& expr) {
    resolve(*expr.receiver);

    return std::monostate{};
}

Value Resolver::visitGroupingExpr(const Expr::Grouping& expr) {
    resolve(*expr.expression);

    return std::monostate{};
}

Value Resolver::visitLiteralExpr(const Expr::Literal& expr) {
    return std::monostate{};
}

Value Resolver::visitLogicalExpr(const Expr::Logical& expr) {
    resolve(*expr.left);
    resolve(*expr.right);

    return std::monostate{};
}

Value Resolver::visitSetExpr(const Expr::Set& expr) {
    resolve(*expr.value);
    resolve(*expr.receiver);

    return std::monostate{};
}

Value Resolver::visitUnaryExpr(const Expr::Unary& expr) {
    resolve(*expr.right);

    return std::monostate{};
}

Value Resolver::visitVarExpr(const Expr::Variable& expr) {
    if (!scopes.empty()) {
        auto& scope = scopes.back();
        auto it = scope.find(expr.name.lexeme);
        if (it != scope.end() && it->second == false) {  
            Runtime::error(
                expr.name,
                "Can't read local variable in its own initializer."
            );
        }
    }

    resolveLocal(expr, expr.name);

    return std::monostate{};
}

// ----------- Helper Functions -----------
void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>>& statements){
    for (auto& statement: statements) {
        resolve(*statement);
    }
}

void Resolver::resolve(Stmt& stmt){
    stmt.accept(*this);
}

void Resolver::resolve(Expr& expr){
    expr.accept(*this);
}

void Resolver::beginScope() {
    scopes.push_back(std::unordered_map<std::string, bool>{});
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(const Token& name) {
    if (scopes.empty()) return;

    auto& scope = scopes.back();
    
    if (scope.count(name.lexeme)) {
        Runtime::error(
            name,
            "Already a variable with this name in this scope.");
    }

    scope[name.lexeme] = false;
}

void Resolver::define(const Token& name) {
    if (scopes.empty()) return;

    auto& scope = scopes.back();
    scope[name.lexeme] = true;
}

void Resolver::resolveLocal(const Expr& expr, const Token& name) {
    for (int i = scopes.size() - 1; i>=0; i--) {
        auto& scope = scopes[i];
        if (scope.count(name.lexeme)) {
            interpreter.resolve(expr, scopes.size() - 1 - i);

            return;
        }
    }
}

void Resolver::resolveFunction(
    const Stmt::Function& function,
    const FunctionType& type) 
{
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();

    for (const Token& param: function.params) {
        declare(param);
        define(param);
    }

    resolve(function.body);

    endScope();

    currentFunction = enclosingFunction;
}
