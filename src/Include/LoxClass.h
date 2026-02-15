#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "../Include/LoxCallable.h"
#include "LoxFunction.h"

class LoxInstance;  

class LoxClass : public LoxCallable {
public:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;

    LoxClass(
        std::string name,
        std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods)
    : name(std::move(name)), methods(std::move(methods)) {}

    std::shared_ptr<LoxFunction> findMethod(const std::string& name) {
        auto it = methods.find(name);
        if (it != methods.end()) {
            return it->second;
        }
        return nullptr;
    }

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
