#pragma once

#ifndef PARSER_H 
#define PARSER_H 

#include <string>
#include <vector>

#include "../Token/TokenType.h"
#include "../Token/Token.h"
#include "../Lox/Expr.h"
#include "../Lox/Lox.h"

class Parser {
private:
    // Array of Tokens
    std::vector<Token> tokens;
    int current = 0;

    // Expression Grammar Function (Low -> High)
    Expr* expression();
    Expr* equality();
    Expr* comparison();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* primary();

    // Helper Function
    bool match(std::vector<TokenType> types);
    bool check(TokenType type);
    bool isAtEnd();
    Token consume(TokenType type, std::string message);
    Token advance();
    Token peek();
    Token previous();

public:
    // Constructor
    Parser(std::vector<Token> tokens);
};

#endif
