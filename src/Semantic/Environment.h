#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../Runtime/Value.h"
#include "../Token/Token.h"
#include <unordered_map>

class Environment {
public:
    Environment()
    : enclosing(nullptr) {}

    Environment(Environment* enclosing)
    : enclosing(enclosing) {}

    Environment* const enclosing;
    std::unordered_map<std::string, Value> values;

    Value get(Token name);
    void define(std::string name, Value value);
    void assign(Token name, Value value);
};

#endif
