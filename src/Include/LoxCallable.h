#pragma once

#include "../Runtime/Value.h"

#include <vector>
#include <string>

class Interpreter;

class LoxCallable {
public:
    virtual Value call(Interpreter* interpreter,
                       const std::vector<Value>& arguments) = 0;

    virtual int arity() const = 0; 
    virtual std::string toString() const = 0;

    virtual ~LoxCallable() = default;
};

