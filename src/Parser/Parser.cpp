#include "Parser.h"
#include <memory>
#include <vector>

// ------------ Rules ------------
std::unique_ptr<Expr> Parser::expression(){
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment() {
    std::unique_ptr<Expr> expr = logicOr();

    if (match({EQUAL})) {
        Token equals = previous();
        std::unique_ptr<Expr> value = assignment();

        if (auto var = dynamic_cast<Expr::Variable*>(expr.get())) {
            Token name = var->name;
            return std::make_unique<Expr::Assign>(name, std::move(value));
        }

        throw error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::logicOr() {
    std::unique_ptr<Expr> expr = logicAnd();

    while (match({OR})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = logicAnd();

        expr = std::make_unique<Expr::Logical>(
            std::move(expr),
            std::move(operator_),
            std::move(right)
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::logicAnd() {
    std::unique_ptr<Expr> expr = equality();

    while (match({AND})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = equality();

        expr = std::make_unique<Expr::Logical>(
            std::move(expr),
            std::move(operator_),
            std::move(right)
        );
    }

    return expr;
}

std::unique_ptr<Stmt> Parser::declaration() {
    try {
        if (match({CLASS}))
            return classDeclaration();

        if (match({FUN}))
            return function("function");

        if (match({VAR}))
            return varDeclaration();

        return statement();
    }
    catch (const ParseError&) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::classDeclaration() {
    Token name = consume(IDENTIFIER, "Expect class name.");

    std::unique_ptr<Expr::Variable> superclass = nullptr;
    if (match({LESS})) {
        Token superName = consume(IDENTIFIER, "Expect superclass name.");
        superclass = std::make_unique<Expr::Variable>(superName);
    }

    consume(LEFT_BRACE, "Expect '{' before class body.");

    std::vector<std::unique_ptr<Stmt::Function>> methods;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        methods.push_back(function("method"));
    }

    consume(RIGHT_BRACE, "Expect '}' after class body.");

    return std::make_unique<Stmt::Class>(
        std::move(name),
        std::move(superclass),
        std::move(methods)
    );
}

std::unique_ptr<Expr> Parser::equality(){
    // equality → comparison ( ( "!=" | "==" ) comparison )* ;
    std::unique_ptr<Expr> expr = comparison();

    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Expr::Binary>(std::move(expr), operator_, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison(){
    // comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
    std::unique_ptr<Expr> expr = term();

    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Expr::Binary>(std::move(expr), operator_, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term(){
    // term → factor ( ( "-" | "+" ) factor )* ;
    std::unique_ptr<Expr> expr = factor();

    while (match({MINUS, PLUS})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<Expr::Binary>(std::move(expr), operator_, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor(){
    // factor → unary ( ( "/" | "*" ) unary )* ;
    std::unique_ptr<Expr> expr = unary();

    while (match({SLASH, STAR})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<Expr::Binary>(std::move(expr), operator_, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary(){
    // unary → ( "!" | "-" ) unary | call ;
    while (match({BANG, MINUS})) {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Expr::Unary>(operator_, std::move(right));
    }

    return call();
}

std::unique_ptr<Expr> Parser::call(){
    // call → primary ( "(" arguments? ")" )* ;
    // arguments → expression ( "," expression )* ;
    std::unique_ptr<Expr> expr = primary();
    while (true) {
        if (match({LEFT_PAREN})) {
            expr = finishCall(expr);
        }
        else {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr>& callee){
    std::vector<std::unique_ptr<Expr>> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments");
            }
            arguments.push_back(expression());
        } while (match({COMMA}));
    }

    Token paren = consume(RIGHT_PAREN,
                          "Expect ')' after arguments.");

    return std::make_unique<Expr::Call>(std::move(callee), 
                                        paren, 
                                        std::move(arguments));
}

std::unique_ptr<Expr> Parser::primary(){
    // primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" | IDENTIFIER ;
    if (match({FALSE})) return std::make_unique<Expr::Literal>(false);
    if (match({TRUE})) return std::make_unique<Expr::Literal>(true);
    if (match({NIL})) return std::make_unique<Expr::Literal>(std::monostate{});

    if (match({NUMBER, STRING})) {
        return std::make_unique<Expr::Literal>(previous().literal);
    }

    if (match({IDENTIFIER})) {
        return std::make_unique<Expr::Variable>(previous());
    }

    if(match({LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Expr::Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }

    return statements;
}

// ----------- Nested Class -----------
Parser::ParseError Parser::error(const Token& token, const std::string& message) {
    Runtime::error(token.line, message);
    return ParseError(token, message);
}

// Synchronize after panic
// Checks for a new statement
void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        // These Suggests beginning of a new statement
        if (previous().type == SEMICOLON) return;

        // If you hit any of these return
        // cause we have hit a new statement
        // synchronization completed
        switch (peek().type) { 
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

// ------------ Private Helpers ------------
std::unique_ptr<Stmt> Parser::statement() {
    if (match({FOR}))
        return forStatement();

    if (match({IF}))
        return ifStatement();

    if (match({PRINT}))
        return printStatement();

    if (match({RETURN}))
        return returnStatement();

    if (match({WHILE}))
        return whileStatement();

    if (match({LEFT_BRACE})){
        std::vector<std::unique_ptr<Stmt>> statements = block();
        return std::make_unique<Stmt::Block>(std::move(statements));
    }

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::forStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    std::unique_ptr<Stmt> initializer;

    // Var initializer
    if (match({SEMICOLON})) {
        initializer = nullptr;
    }
    else if (match({VAR})) {
        initializer = varDeclaration();
    }
    else {
        initializer = expressionStatement();
    }

    // Loop condition
    std::unique_ptr<Expr> condition = nullptr;
    if (!check(SEMICOLON)) {
        condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");
    
    // Var increment
    std::unique_ptr<Expr> increment = nullptr;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");

    // Syntactic Sugar: For -> While 
    std::unique_ptr<Stmt> body = statement();

    if (increment != nullptr) {
        std::vector<std::unique_ptr<Stmt>> statements;

        statements.push_back(std::move(body));
        statements.push_back(
            std::make_unique<Stmt::Expression>(std::move(increment))
        );

        body = std::make_unique<Stmt::Block>(std::move(statements));
    }

    if (condition == nullptr)
        condition = std::make_unique<Expr::Literal>(true);

    body = std::make_unique<Stmt::While>
        (std::move(condition),
         std::move(body));

    if (initializer != nullptr) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<Stmt::Block>(std::move(statements));
    }

    return body;
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");

    return statements;
}

std::unique_ptr<Stmt> Parser::ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after 'if'.");

    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;

    if (match({ELSE})) {
        elseBranch = statement();
    }

    return std::make_unique<Stmt::If>
    (
        std::move(condition), 
        std::move(thenBranch),
        std::move(elseBranch)
    );
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value");

    return std::make_unique<Stmt::Print>(std::move(value));
}

std::unique_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::unique_ptr<Expr> value = nullptr;

    if (!check(SEMICOLON)) {
        value = expression();
    }

    consume(SEMICOLON, "Expect ';' after return value");

    return std::make_unique<Stmt::Return>(keyword, std::move(value));
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");

    std::unique_ptr<Expr> initializer = nullptr;

    if (match({EQUAL})) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration");
    return std::make_unique<Stmt::Var>(name, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::whileStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(LEFT_PAREN, "Expect ')' after condition");
    std::unique_ptr<Stmt> body = statement();

    return std::make_unique<Stmt::While>
    (std::move(condition),
     std::move(body));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> expr = expression();
    consume(SEMICOLON, "Expect ';' after expression");

    return std::make_unique<Stmt::Expression>(std::move(expr));
}

bool Parser::match(std::vector<TokenType> types){
    for(const TokenType& type: types) {
        if(check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

std::unique_ptr<Stmt::Function> Parser::function(std::string kind) {
    Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    
    std::vector<Token> parameters;
    if (!check(RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments");
            }

            parameters.push_back(
                consume(IDENTIFIER, "Expect parameter name."));
        } while (match({COMMA}));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");

    // Body
    consume(LEFT_BRACE, "Expect '{' before" + kind + " body.");
    std::vector<std::unique_ptr<Stmt>> body = block();

    return std::make_unique<Stmt::Function>(name, parameters, std::move(body));
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
