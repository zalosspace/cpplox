#include "Interpreter.h"

#include "../Runtime/Runtime.h"
#include "../Include/LoxClass.h"
#include "../Include/LoxCallable.h"
#include "../Include/LoxFunction.h"
#include "../Include/LoxInstance.h"

#include <memory>
#include <string>
#include <variant>

// ---------- Public API ----------
void Interpreter::interpret(
    const std::vector<std::unique_ptr<Stmt>> &statements) {
  try {
    for (const auto &statement : statements) {
      execute(*statement);
    }
  } catch (RuntimeError error) {
    Runtime::runtimeError(error);
  }
}

// ---------- Override Visitor Function ----------
Value Interpreter::visitLiteralExpr(const Expr::Literal &expr) { 
    return expr.value;
}

Value Interpreter::visitLogicalExpr(const Expr::Logical &expr) { 
    Value left = evaluate(*expr.left);

    // Or
    if (expr.operator_.type == TokenType::OR){
        if (isTruthy(left)) 
            return left;
    }
    // And
    else { 
        if (!isTruthy(left)) 
            return left;
    }

    return evaluate(*expr.right);
}

Value Interpreter::visitUnaryExpr(const Expr::Unary &expr) {
  Value right = evaluate(*expr.right);

  switch (expr.operator_.type) {
  case MINUS: {
    checkNumberOperand(expr.operator_, right);
    double value = std::get<double>(right);
    return -value;
  }

  case BANG: {
    bool value = isTruthy(right);
    return !value;
  }
  }

  // Unreachable.
  return std::monostate{};
}

Value Interpreter::visitGroupingExpr(const Expr::Grouping &expr) {
  return expr.expression->accept(*this);
}

Value Interpreter::visitBinaryExpr(const Expr::Binary &expr) {
  Value left = evaluate(*expr.left);
  Value right = evaluate(*expr.right);

  switch (expr.operator_.type) {
  // Comparison Operator
  case GREATER:
    checkNumberOperands(expr.operator_, left, right);
    return std::get<double>(left) > std::get<double>(right);

  case GREATER_EQUAL:
    checkNumberOperands(expr.operator_, left, right);
    return std::get<double>(left) >= std::get<double>(right);

  case LESS:
    checkNumberOperands(expr.operator_, left, right);
    return std::get<double>(left) < std::get<double>(right);

  case LESS_EQUAL:
    checkNumberOperands(expr.operator_, left, right);
    return std::get<double>(left) <= std::get<double>(right);

  // Binary Operator
  case MINUS:
    checkNumberOperands(expr.operator_, left, right);
    return std::get<double>(left) - std::get<double>(right);

  case PLUS: {
    if (std::get_if<double>(&left) && std::get_if<double>(&right)) {
      return std::get<double>(left) + std::get<double>(right);
    }

    if (std::get_if<std::string>(&left) && std::get_if<std::string>(&right)) {
      return std::get<std::string>(left) + std::get<std::string>(right);
    }

    throw RuntimeError(expr.operator_,
                       "Operands must be two numbers or two strings.");
  }

  case SLASH:
    checkNumberOperands(expr.operator_, left, right);
    return std::get<double>(left) / std::get<double>(right);

  case STAR:
    checkNumberOperands(expr.operator_, left, right);
    return std::get<double>(left) * std::get<double>(right);

  // Equality Operator
  case BANG_EQUAL:
    return !isEqual(left, right);
  case EQUAL_EQUAL:
    return isEqual(left, right);
  }

  // Unreachable.
  return std::monostate{};
}

Value Interpreter::visitVarExpr(const Expr::Variable& expr) {
    return lookUpVariable(expr.name, expr);
}

Value Interpreter::lookUpVariable(
    const Token& name, 
    const Expr::Variable& expr)
{
    auto it = locals.find(&expr);
    
    if (it != locals.end()) {
        int distance = it->second;
        return environment->getAt(distance, name.lexeme);
    }
    else {
        return globals->get(name);
    }
}

Value Interpreter::visitVarStmt(const Stmt::Var& stmt) {
    Value value = std::monostate{};

    if (stmt.initializer) {
        value = evaluate(*stmt.initializer.get());
    }

    environment->define(stmt.name.lexeme, value);
    return std::monostate{};
}

Value Interpreter::visitWhileStmt(const Stmt::While& stmt) {
    while (isTruthy(evaluate(*stmt.condition))) {
        execute(*stmt.body);
    }

    return std::monostate{};
}

// ---------- Private Function ----------
// ---------- Helper Function ----------
Value Interpreter::evaluate(const Expr& expr) { 
    return expr.accept(*this);
}

void Interpreter::execute(const Stmt& stmt) { 
    stmt.accept(*this); 
}

void Interpreter::resolve(const Expr& expr, int depth) {
    locals[&expr] = depth;
}

void Interpreter::executeBlock(
    const std::vector<std::unique_ptr<Stmt>>& statements,
    std::shared_ptr<Environment> newEnv)
{
    auto previous = environment;

    try {
        environment = newEnv;

        for (const auto& stmt : statements) {
            execute(*stmt);
        }
    }
    catch (...) {
        environment = previous;
        throw;
    }

    environment = previous;
}

bool Interpreter::isTruthy(const Value &value) {
  if (std::holds_alternative<std::monostate>(value))
    return false;

  if (auto b = std::get_if<bool>(&value))
    return *b;

  return true;
}

bool Interpreter::isEqual(const Value &a, const Value &b) {
  // std::variant handles type checking for yuh.
  return a == b;
}

std::string Interpreter::stringify(const Value &value) {
  if (std::holds_alternative<std::monostate>(value))
    return "nil";

  if (auto d = std::get_if<double>(&value)) {
    std::string text = std::to_string(*d);

    if (text.size() >= 2 && text.substr(text.size() - 2) == ".0") {
      text = text.substr(0, text.size() - 2);
    }

    return text;
  }

  if (auto b = std::get_if<bool>(&value)) {
    return *b ? "true" : "false";
  }

  if (auto b = std::get_if<std::string>(&value)) {
    return *b;
  }

    if (auto callable = std::get_if<std::shared_ptr<LoxCallable>>(&value)) {
        return (*callable)->toString();
    }

    if (auto instance = std::get_if<std::shared_ptr<LoxInstance>>(&value)) {
        return (*instance)->toString();
    }

  return "nil";
}

void Interpreter::checkNumberOperand(Token operator_, Value operand) {
  if (auto d = std::get_if<double>(&operand))
    return;

  throw RuntimeError(operator_, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token operator_, Value left,
                                      Value right) {
  if (std::get_if<double>(&left) && std::get_if<double>(&right)) {
    return;
  }

  throw RuntimeError(operator_, "Operand must be a numbers.");
}

// Override
Value Interpreter::visitExpressionStmt(const Stmt::Expression &stmt) {
  evaluate(*stmt.expression);

  return std::monostate{};
}

Value Interpreter::visitFunctionStmt(const Stmt::Function& stmt) {
    // Wrap the function declaration in a LoxFunction
    std::shared_ptr<LoxCallable> function =
        std::make_shared<LoxFunction>(&stmt, environment);

    // Store it in the environment as a Value
    environment->define(stmt.name.lexeme, Value(function));

    return std::monostate{}; // functions don't return a value
}

Value Interpreter::visitIfStmt(const Stmt::If& stmt) {
    if (isTruthy(evaluate(*stmt.condition))){
        execute(*stmt.thenBranch);
    }
    else if (stmt.elseBranch != nullptr){
        execute(*stmt.elseBranch);
    }

    return std::monostate{};
}

Value Interpreter::visitPrintStmt(const Stmt::Print& stmt) {
  Value value = evaluate(*stmt.expression);
  std::cout << stringify(value) << '\n';

  return std::monostate{};
}

Value Interpreter::visitReturnStmt(const Stmt::Return& stmt) {
    Value value = std::monostate{};

    if (stmt.value != nullptr)
        value = evaluate(*stmt.value);

    // Returns value to the caller
    throw Return(value);
}

Value Interpreter::visitAssignExpr(const Expr::Assign& expr) {
    Value value = evaluate(*expr.value);

    auto it = locals.find(&expr);

    if (it != locals.end()) {
        int distance = it->second;
        environment->assignAt(distance, expr.name, value);
    } else {
        globals->assign(expr.name, value);
    }

    return value;
}

Value Interpreter::visitCallExpr(const Expr::Call& expr) {
    Value callee = evaluate(*expr.callee);

    std::vector<Value> arguments;
    for (const auto& argument: expr.arguments){
        arguments.push_back(evaluate(*argument));
    }

    if (!(std::holds_alternative<std::shared_ptr<LoxCallable>>(callee))) {
        throw RuntimeError(expr.paren,
                           "Can only call function and classes.");
    }

    std::shared_ptr<LoxCallable> function = std::get<std::shared_ptr<LoxCallable>>(callee);

    if (arguments.size() != function->arity()) {
        throw RuntimeError(expr.paren,
                           "Expected " + std::to_string(function->arity()) +
                           " arguments but got " + 
                           std::to_string(arguments.size()) + ".");
    }

    return function->call(this, arguments);
}

Value Interpreter::visitBlockStmt(const Stmt::Block& stmt) {
    std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(environment);
    executeBlock(stmt.statements, newEnv);
    return std::monostate{};
}

Value Interpreter::visitClassStmt(const Stmt::Class& stmt) {
    environment->define(stmt.name.lexeme, std::monostate{});

    auto klass = std::make_shared<LoxClass>(stmt.name.lexeme);

    environment->assign(stmt.name, klass);

    return std::monostate{};
}
