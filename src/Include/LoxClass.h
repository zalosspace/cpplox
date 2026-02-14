#pragma once

#include <string>
#include <memory>
#include "../Include/LoxCallable.h"

class LoxInstance;  

class LoxClass : public LoxCallable {
public:
    std::string name;

    LoxClass(std::string name)
        : name(std::move(name)) {}

    Value call(
        Interpreter* interpreter,
        const std::vector<Value>& arguments
    ) override;

    int arity() const override {
        return 0;
    }

    std::string toString() const override {
        return name;
    }
};
