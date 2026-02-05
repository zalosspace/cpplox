#include "Token.h"
#include "TokenType.h"
#include <iostream>
#include <string>
#include <any>
#include <variant>

std::ostream& operator << (std::ostream& os, const Token& token){
    os << token.toString();
    return os;
}

// Constructor
Token::Token(TokenType type, std::string lexeme, Value literal, int line):
    type(type), lexeme(lexeme), literal(literal), line(line) {}

std::string Token::toString() const {
    std::string literalStr;
    if(!std::holds_alternative<std::monostate>(literal)){
        if(std::get_if<std::string>(&literal)) literalStr = std::get<std::string>(literal);
        if(std::get_if<double>(&literal)) literalStr = std::to_string(std::get<double>(literal));
    }

    return tokenTypeToString(type) + " " + lexeme + " " + literalStr;
}
