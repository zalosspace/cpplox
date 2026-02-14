#pragma once

#include <string>

class LoxObject {
public:
    virtual std::string toString() const = 0;
    virtual ~LoxObject() = default;
};
