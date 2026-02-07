#pragma once

#ifndef PARSER_H 
#define PARSER_H 

#include <string>
#include <vector>
#include <stdexcept>

#include "../Token/TokenType.h"
#include "../Token/Token.h"
#include "../Lox/Expr.h"
#include "../Lox/Stmt.h"

class Parser {
    public:
        // Constructor
        Parser(std::vector<Token> tokens);

        std::vector<std::unique_ptr<Stmt>> parse();

        std::unique_ptr<Stmt> statement();
        std::unique_ptr<Stmt> declaration();
        std::unique_ptr<Stmt> printStatement();
        std::unique_ptr<Stmt> varDeclaration();
        std::unique_ptr<Stmt> expressionStatement();
        std::unique_ptr<Stmt> block();

    private:
        // Array of Tokens
        std::vector<Token> tokens;
        int current = 0;

        // Expression Grammar Function (Low -> High)
        //expression → assignment ;
        std::unique_ptr<Expr> expression();
        // assignment → IDENTIFIER "=" assignment | equality;
        std::unique_ptr<Expr> assignment();
        // equality → comparison ( ( "!=" | "==" ) comparison )* ;
        std::unique_ptr<Expr> equality();
        // comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
        std::unique_ptr<Expr> comparison();
        // term → factor ( ( "-" | "+" ) factor )* ;
        std::unique_ptr<Expr> term();
        // factor → unary ( ( "/" | "*" ) unary )* ;
        std::unique_ptr<Expr> factor();
        // unary → ( "!" | "-" ) unary | primary ;
        std::unique_ptr<Expr> unary();
        // primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
        std::unique_ptr<Expr> primary();

        // Helper Function
        bool match(std::vector<TokenType> types);
        bool check(TokenType type);
        bool isAtEnd();
        Token consume(TokenType type, std::string message);
        Token advance();
        Token peek();
        Token previous();

        // Nested Private Class
        class ParseError: public std::runtime_error {
            public:
            ParseError(const Token& token, const std::string& message)
                : std::runtime_error(message), token(token) {}

            Token token;
        };

        ParseError error(const Token& token, const std::string& message);

        // Synchronize after panic
        void synchronize();
};

#endif
