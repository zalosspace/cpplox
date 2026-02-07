#include "Environment.h"
#include "../Interpreter/RuntimeError.h"

// Searches from nested to parent
Value Environment::get(Token name){
    if (values.count(name.lexeme)) {
        return values[name.lexeme];
    }

    if (enclosing != nullptr) 
        return enclosing->get(name);

    throw new RuntimeError(name,
                           "Undefined variable '" + name.lexeme + "'.");
}

void Environment::define(std::string name, Value value){
    values[name] = value;
}

void Environment::assign(Token name, Value value) {
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
