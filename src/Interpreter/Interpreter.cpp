#include "Interpreter.h"
#include "../Lox/Expr.h"
#include "../Lox/Lox.h"
#include "../Semantic/Environment.h"
#include "RuntimeError.h"
#include <iostream>
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
    Lox::runtimeError(error);
  }
}

// ---------- Override Visitor Function ----------
Value Interpreter::visitLiteralExpr(const Literal &expr) { return expr.value; }

Value Interpreter::visitUnaryExpr(const Unary &expr) {
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

Value Interpreter::visitGroupingExpr(const Grouping &expr) {
  return expr.expression->accept(*this);
}

Value Interpreter::visitBinaryExpr(const Binary &expr) {
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

Value Interpreter::visitVarStmt(const Stmt::Var& stmt) {
    Value value = std::monostate{};

    if (stmt.initializer) {
        value = evaluate(*stmt.initializer.get());
    }

    environment->define(stmt.name.lexeme, value);
    return std::monostate{};
}

Value Interpreter::visitVarExpr(const Variable& expr) {
    return environment->get(expr.name);
}

// ---------- Private Function ----------
// ---------- Helper Function ----------
Value Interpreter::evaluate(const Expr &expr) { return expr.accept(*this); }

void Interpreter::execute(const Stmt &stmt) { 
    stmt.accept(*this); 
}

void Interpreter::executeBlock(
    const std::vector<std::unique_ptr<Stmt>>& statements,
    Environment& newEnv)
{
    // RAII guard to restore environment
    struct EnvironmentGuard {
        Environment** envPtr;
        Environment* previous;
        
        EnvironmentGuard(Environment** env, Environment* prev) 
            : envPtr(env), previous(prev) {}
        
        ~EnvironmentGuard() { *envPtr = previous; }
    };
    
    EnvironmentGuard guard(&environment, environment);
    environment = &newEnv;
    
    for (const auto& stmt : statements) {
        execute(*stmt);
    }
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

Value Interpreter::visitPrintStmt(const Stmt::Print &stmt) {
  Value value = evaluate(*stmt.expression);
  std::cout << stringify(value) << '\n';

  return std::monostate{};
}


Value Interpreter::visitAssignExpr(const Assign& expr) {
    Value value = evaluate(*expr.value);
    environment->assign(expr.name, value);

    return value;
}

Value Interpreter::visitBlockStmt(const Stmt::Block& stmt) {
    Environment newEnv(environment);
    executeBlock(stmt.statements, newEnv);
    return std::monostate{};
}

