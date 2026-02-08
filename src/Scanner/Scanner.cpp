#include "Scanner.h"
#include "../Lox/Lox.h"
#include "../Token/TokenType.h"
#include "../Token/Token.h"
#include <vector>
#include <string>

// NOTE: It fails to handle string literals as of now
const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::FALSE},
    {"for",    TokenType::FOR},
    {"fun",    TokenType::FUN},
    {"if",     TokenType::IF},
    {"nil",    TokenType::NIL},
    {"or",     TokenType::OR},
    {"print",  TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super",  TokenType::SUPER},
    {"this",   TokenType::THIS},
    {"true",   TokenType::TRUE},
    {"var",    TokenType::VAR},
    {"while",  TokenType::WHILE}
};

// Constructor
Scanner::Scanner(std::string source): source(source){};

// Array of Tokens
std::vector<Token> Scanner::scanTokens(){
    // Keep scanning until we hit end of file 
    while(!isAtEnd()){
        // point the scanner to current lexeme beginning
        start=current;
        scanToken();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", Value{}, line));
    return tokens;
}

void Scanner::scanToken(){
    char c = advance();

    switch (c) {
        // single character match
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;

        // multiple character match
        case '!':
                  addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                  break;
        case '=':
                  addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                  break;
        case '<':
                  addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                  break;
        case '>':
                  addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                  break;
        case '/':
                  if(match('/')){ // check if double-slash: comment
                      while(peek() != '\n' && !isAtEnd()) advance();
                  }
                  else{ // divisor
                      addToken(TokenType::SLASH);
                  }
                  break;

        // special character 
        case ' ':
        case '\r':
        case '\t':
                  // Ignore whitespace
                  break;
        case '\n':
                  line++;
                  break;

        // literals
        default: 
                  if(isDigit(c)) number();
                  else if(isAlpha(c)) identifier();
                  else Lox::error(line, "Unexpected character");
                  break;
    }
}

void Scanner::identifier(){
    while(isAlphaNumeric(peek())){
        advance();
    }

    std::string text = source.substr(start, current-start);

    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;

    addToken(type);
}

bool Scanner::isAlpha(char c){
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
           (c == '_'); 
}

bool Scanner::isAlphaNumeric(char c){
    return isAlpha(c) || isDigit(c);
}

void Scanner::number(){
    while(isDigit(peek())) advance();

    // check for decimal
    if(peek() == '.' && isDigit(peekNext())){
        advance(); // consume .
        while(isDigit(peek())) advance();
    }

    addToken(TokenType::NUMBER, std::stod(source.substr(start, current-start)));
}

bool Scanner::isDigit(char c){
    return c >= '0' && c <= '9';
}

void Scanner::String(){
    while(peek() != '"' && !isAtEnd()){
        if(peek() == '\n') line++;
        advance();
    }

    if(isAtEnd()){
        Lox::error(line, "Unterminated string.");
        return;
    }

    advance();

    std::string value = source.substr(start+1, current-start-1);
    addToken(TokenType::STRING, value);

    return;
}

// Function to check character for multiple character lexeme 
bool Scanner::match(char expected){
    if(isAtEnd()) return false;
    if(source[current] != expected) return false;

    current++;
    return true;
}

char Scanner::peek(){
    // Return the next character
    // Return '\0' if at end of source
    return isAtEnd() ? '\0' : source[current];
}

char Scanner::peekNext(){
    // Return next to next character
    // Return '\0' if at end of source
    if(current+1 >= source.size()) return '\0';
    return source[current+1];
}

char Scanner::advance(){
    current++;
    return source[current-1];
}

bool Scanner::isAtEnd(){
    return current >= source.size();
}

// Function to add token with only its type 
void Scanner::addToken(TokenType type){
    addToken(type, "");
}

// Overloaded Function to add token with type and literal
void Scanner::addToken(TokenType type, Value literal){
    std::string text=source.substr(start, current-start); 
    tokens.push_back(Token(type, text, literal, line));
}
