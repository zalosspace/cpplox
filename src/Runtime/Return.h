#pragma once

#include "Value.h"
#include <stdexcept>

class Return : public std::runtime_error {
public:
    Value value;

    Return(Value value)
    : std::runtime_error("return"), value(value) {}
};
