#ifndef SCANNER_H
#define SCANNER_H

#include "../Token/TokenType.h"
#include "../Token/Token.h"
#include <cstddef>
#include <vector>
#include <string>
#include <any>
#include <unordered_map>

class Scanner {
private:
    std::size_t start = 0;
    std::size_t current = 0;
    int line = 1;
    std::string source;
    std::vector<Token> tokens;
    static const std::unordered_map<std::string, TokenType> keywords;

public:
    Scanner(std::string source);

    std::vector<Token> scanTokens();

private:
    void scanToken();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    bool isDigit(char c);
    void identifier();
    void number();
    void String();
    bool match(char expected);
    char peek();
    char peekNext();
    char advance();
    bool isAtEnd();
    void addToken(TokenType type);
    void addToken(TokenType type, Value literal);
};

#endif

