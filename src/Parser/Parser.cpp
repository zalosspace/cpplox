#include "Parser.h"

// Constructor
Parser::Parser(std::vector<Token> tokens): tokens(tokens){};

// ------------ Rules ------------
Expr* Parser::expression(){
    return equality();
}

Expr* Parser::equality(){
    // equality → comparison ( ( "!=" | "==" ) comparison )* ;
    Expr* expr = comparison();

    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        Token operator_ = previous();
        Expr* right = comparison();
        expr = new Binary(expr, operator_, right);
    }
    return expr;
}

Expr* Parser::comparison(){
    // comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
    Expr* expr = term();

    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token operator_ = previous();
        Expr* right = term();
        expr = new Binary(expr, operator_, right);
    }
    return expr;
}

Expr* Parser::term(){
    // term → factor ( ( "-" | "+" ) factor )* ;
    Expr* expr = factor();

    while (match({MINUS, PLUS})) {
        Token operator_ = previous();
        Expr* right = factor();
        expr = new Binary(expr, operator_, right);
    }
    return expr;
}

Expr* Parser::factor(){
    // factor → unary ( ( "/" | "*" ) unary )* ;
    Expr* expr = unary();

    while (match({SLASH, STAR})) {
        Token operator_ = previous();
        Expr* right = unary();
        expr = new Binary(expr, operator_, right);
    }
    return expr;
}

Expr* Parser::unary(){
    // unary → ( "!" | "-" ) unary | primary ;
    while (match({BANG, MINUS})) {
        Token operator_ = previous();
        Expr* right = unary();
        return new Unary(operator_, right);
    }

    return primary();
}

Expr* Parser::primary(){
    // primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
    if (match({FALSE})) return new Literal(false);
    if (match({TRUE})) return new Literal(true);
    if (match({NIL})) return new Literal(nullptr);

    if (match({NUMBER, STRING})) {
        return new Literal(previous().literal);
    }

    if(match({LEFT_PAREN})) {
        Expr* expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping(expr);
    }

    return Parser::primary();
}

// ------------ Private Helpers ------------
bool Parser::match(std::vector<TokenType> types){
    for(const TokenType& type: types) {
        if(check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

// Advance if we have expected token
Token Parser::consume(TokenType type, std::string message){
    if(check(type)) return advance();

    Lox::error(peek(), message);
}

// Check If The Next Token Matches
bool Parser::check(TokenType type){
    if(isAtEnd()) return false;
    return peek().type == type;
}

// Consumes the current token and return it
Token Parser::advance(){
    if(!isAtEnd()) current++;
    return previous();
}

// Checks for END_OF_FILE 
bool Parser::isAtEnd() {
    return peek().type == END_OF_FILE;
}

// Returns the next token
Token Parser::peek() {
    return tokens[current];
}

// Returns the previous token
Token Parser::previous() {
    return tokens[current-1];
}
