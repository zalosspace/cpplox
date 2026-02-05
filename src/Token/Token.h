#ifndef TOKEN_H
#define TOKEN_H

#include "TokenType.h"
#include "../Lox/Value.h"
#include <ostream>
#include <string>
#include <any>

class Token {
public:
    TokenType type;
    std::string lexeme;
    Value literal;
    int line;

    Token(TokenType type, std::string lexeme, Value literal, int line);

public:
    std::string toString() const;
};

std::ostream& operator << (std::ostream& os, const Token& token);

#endif
