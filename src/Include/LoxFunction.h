#pragma once

#include "LoxCallable.h"
#include "../Runtime/Stmt.h"
#include "../Runtime/Return.h"
#include "../Semantic/Environment.h"
#include "../Interpreter/Interpreter.h"

#include <memory>
#include <string>
#include <variant>

class LoxFunction : public LoxCallable {
public:
    const Stmt::Function* declaration;
    std::shared_ptr<Environment> closure;

    LoxFunction(const Stmt::Function* declaration,
                std::shared_ptr<Environment> closure)
    : declaration(declaration), closure(closure) {}

    int arity() const override {
        return declaration->params.size();
    }

    Value call(Interpreter* interpreter, const std::vector<Value>& arguments) override { 
        std::shared_ptr<Environment> environment = 
            std::make_shared<Environment>(closure);

        for (int i=0; i < declaration->params.size(); i++) {
            environment->define(
                declaration->params[i].lexeme,
                arguments[i]);
        }
        
        try {
            interpreter->executeBlock(
                declaration->body,
                environment
            );
        }
        catch (Return returnValue) {
            return returnValue.value;
        }

        return std::monostate{};
    };

private:
    std::string toString() const override {
        return "<fn " + declaration->name.lexeme + ">";
    }
};
