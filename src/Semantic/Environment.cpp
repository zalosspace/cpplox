#include "Environment.h"
#include "../Interpreter/RuntimeError.h"

// Searches from nested to parent
Value Environment::get(const Token& name){
    if (values.count(name.lexeme)) {
        return values[name.lexeme];
    }

    if (enclosing != nullptr) 
        return enclosing->get(name);

    throw new RuntimeError(name,
                           "Undefined variable '" + name.lexeme + "'.");
}

void Environment::define(const std::string& name, const Value& value){
    values[name] = value;
}

Environment* Environment::ancestor(int distance) {
    Environment* environment = this;

    for (int i=0; i<distance; i++) {
        environment = environment->enclosing.get();
    }

    return environment;
}

Value Environment::getAt(int distance, const std::string& name) {
    return ancestor(distance)->values[name];
}

void Environment::assignAt(int distance, const Token& name, Value value) {
   ancestor(distance)->values[name.lexeme] = value;
}

void Environment::assign(const Token& name, const Value& value) {
    if (values.count(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }
    
    throw RuntimeError(name, 
                       "Undefined variable '" + name.lexeme + "'.");
}
