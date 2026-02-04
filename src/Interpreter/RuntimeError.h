#pragma once
#include <stdexcept>
#include <string>
#include "../Token/Token.h"

#ifndef RUNTIME_ERROR
#define RUNTIME_ERROR

class RuntimeError : public std::runtime_error {
    public:
        Token token;

        RuntimeError(const Token& token, const std::string& message):
            std::runtime_error(message), token(token) {}
};
#endif
