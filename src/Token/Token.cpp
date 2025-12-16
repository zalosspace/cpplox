#include "Token.h"
#include "TokenType.h"
#include <iostream>
#include <string>
#include <any>

std::ostream& operator << (std::ostream& os, const Token& token){
    os << token.toString();
    return os;
}

// Constructor
Token::Token(TokenType type, std::string lexeme, std::any literal, int line):
    type(type), lexeme(lexeme), literal(literal), line(line) {}

std::string Token::toString() const {
    std::string literalStr;
    if(literal.has_value()){
        if(literal.type() == typeid(std::string)) literalStr = std::any_cast<std::string>(literal);
        if(literal.type() == typeid(double)) literalStr = std::to_string(std::any_cast<double>(literal));
    }

    return tokenTypeToString(type) + " " + lexeme + " " + literalStr;
}

