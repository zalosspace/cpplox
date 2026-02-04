#include "Parser.h"
#include "../Lox/Lox.h"
#include <memory>

// Constructor
Parser::Parser(std::vector<Token> tokens): tokens(tokens){};

// ------------ Rules ------------
std::unique_ptr<Expr> Parser::expression(){
    return equality();
}

std::unique_ptr<Expr> Parser::equality(){
    // equality → comparison ( ( "!=" | "==" ) comparison )* ;
    std::unique_ptr<Expr> expr = comparison();

    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison(){
    // comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
    std::unique_ptr<Expr> expr = term();

    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term(){
    // term → factor ( ( "-" | "+" ) factor )* ;
    std::unique_ptr<Expr> expr = factor();

    while (match({MINUS, PLUS})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor(){
    // factor → unary ( ( "/" | "*" ) unary )* ;
    std::unique_ptr<Expr> expr = unary();

    while (match({SLASH, STAR})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary(){
    // unary → ( "!" | "-" ) unary | primary ;
    while (match({BANG, MINUS})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Unary>(operator_, std::move(right));
    }

    return primary();
}

std::unique_ptr<Expr> Parser::primary(){
    // primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
    if (match({FALSE})) return std::make_unique<Literal>(false);
    if (match({TRUE})) return std::make_unique<Literal>(true);
    if (match({NIL})) return std::make_unique<Literal>(nullptr);

    if (match({NUMBER, STRING})) {
        return std::make_unique<Literal>(previous().literal);
    }

    if(match({LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

std::unique_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (const ParseError&) {
        return nullptr;
    }
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

    throw error(peek(), message);
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

// ----------- Nested Class -----------
Parser::ParseError Parser::error(const Token& token, const std::string& message) {
    Lox::error(token.line, message);
    return ParseError(token, message);
}

// synchronize after panic
// Checks for a new statement
void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        // These Suggests beginning of a new statement
        if (previous().type == SEMICOLON ) return;

        // If you hit any of these return
        // cause we have hit a new statement
        // synchronization completed
        switch (peek().type) { // cleaner than writing a giant if else 
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }

        advance();
    }
}
