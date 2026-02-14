#pragma once

#include "LoxObject.h"
#include "../Token/Token.h"
#include "../Runtime/Value.h"
#include "../Interpreter/RuntimeError.h"

#include <memory>
#include <string>
#include <unordered_map>

class LoxClass;  

class LoxInstance : public LoxObject {
public:
    LoxInstance(std::shared_ptr<LoxClass> klass)
        : klass(std::move(klass)) {}

    Value get(const Token& name);
    void set(const Token& name, const Value& value);

    std::string toString() const override;

private:
    std::shared_ptr<LoxClass> klass;
    std::unordered_map<std::string, Value> fields;
};
