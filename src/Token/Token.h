#ifndef TOKEN_H
#define TOKEN_H

#include "TokenType.h"
#include "../Runtime/Value.h"
#include <ostream>
#include <string>
#include <any>

class Token {
public:
    TokenType type;
    std::string lexeme;
    Value literal;
    int line;

    Token(TokenType type, std::string lexeme, Value literal, int line):
        type(type), lexeme(lexeme), literal(literal), line(line) {}

    std::string toString() const;
};

std::ostream& operator << (std::ostream& os, const Token& token);

#endif
