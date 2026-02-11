#pragma once

#include "../Runtime/Value.h"
#include "../Token/Token.h"

#include <memory>
#include <unordered_map>
#include <string>

class Environment {
public:
    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment> enclosing;

    Environment() = default;

    Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(enclosing) {}

    Value get(const Token& name);
    void define(const std::string& name, const Value& value);
    void assign(const Token& name, const Value& value);
};
