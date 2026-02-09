#include "Lexer.h"

// NOTE: It fails to handle string literals as of now
const std::unordered_map<std::string, TokenType> Lexer::keywords = {
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

// Array of Tokens
std::vector<Token> Lexer::scanTokens(){
    // Keep scanning until EOF
    while(!isAtEnd()){
        // Point the scanner to current lexeme beginning
        start=current;
        scanToken();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", Value{}, line));
    return tokens;
}

void Lexer::scanToken(){
    char c = advance();

    switch (c) {
        // Single character match
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
                  else Runtime::error(line, "Unexpected character");
                  break;
    }
}

void Lexer::identifier(){
    while(isAlphaNumeric(peek())){
        advance();
    }

    std::string text = source.substr(start, current-start);

    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;

    addToken(type);
}

bool Lexer::isAlpha(char c){
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
           (c == '_'); 
}

bool Lexer::isDigit(char c){
    return c >= '0' && c <= '9';
}

bool Lexer::isAlphaNumeric(char c){
    return isAlpha(c) || isDigit(c);
}

void Lexer::number(){
    while(isDigit(peek())) advance();

    // check for decimal
    if(peek() == '.' && isDigit(peekNext())){
        advance(); // consume .
        while(isDigit(peek())) advance();
    }

    addToken(TokenType::NUMBER, std::stod(source.substr(start, current-start)));
}

void Lexer::String(){
    while(peek() != '"' && !isAtEnd()){
        if(peek() == '\n') line++;
        advance();
    }

    if(isAtEnd()){
        Runtime::error(line, "Unterminated string.");
        return;
    }

    advance();

    std::string value = source.substr(start+1, current - start - 1);
    addToken(TokenType::STRING, value);

    return;
}

// Function to check character for multiple character lexeme 
bool Lexer::match(char expected){
    if(isAtEnd()) return false;
    if(source[current] != expected) return false;

    current++;
    return true;
}

// Return the next character
// Return '\0' if at end of source
char Lexer::peek(){
    return isAtEnd() ? '\0' : source[current];
}

// Return next to next character
// Return '\0' if at end of source
char Lexer::peekNext(){
    if(current+1 >= source.size()) return '\0';
    return source[current+1];
}

char Lexer::advance(){
    current++;
    return source[current-1];
}

bool Lexer::isAtEnd(){
    return current >= source.size();
}

// Function to add token with only its type 
void Lexer::addToken(TokenType type){
    addToken(type, "");
}

// Overloaded Function to add token with type and literal
void Lexer::addToken(TokenType type, Value literal){
    std::string text=source.substr(start, current-start); 
    tokens.push_back(Token(type, text, literal, line));
}
