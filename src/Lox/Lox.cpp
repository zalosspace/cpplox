#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

#include "Lox.h"
#include "../Scanner/Scanner.h"
#include "../Parser/Parser.h"
#include "../AstPrinter/AstPrinter.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

bool Lox::hadError = false;

void Lox::runPrompt(){
    while(true){
        string line;
        cout << "> ";

        // Take Input
        if(!getline(cin, line)) break;
        // Run Command
        Lox::run(line);

        // New line = New start
        hadError=false;
    }
}

void Lox::runFile(const string& path){
    // File input stream
    std::ifstream file(path);

    // Handle file read failure
    if(!file.is_open()){
        cerr << "Could not open file: " << path << endl;
        exit(74); // custom exit error code (file read failure)
    }

    // Read entire file into buffer
    std::stringstream buffer;
    buffer << file.rdbuf(); 
    Lox::run(buffer.str());

    // Exit If Error Occurs Anywhere 
    if(hadError) exit(65);
}

void Lox::run(string source){
    // Initialize the scanner
    Scanner scanner(source);

    std::vector<Token> tokens = scanner.scanTokens();
    // Print all the tokens
    for(const Token& token: tokens){
        cout << token << endl;
    }
    
    // Initialize the parser
    Parser parser(tokens);
    Expr* expression = parser.parse();
    if (!expression) {
        std::cerr << "Parser returned nullptr!" << std::endl;
        return;
    }

    std::cout << expression << std::endl;

    // Stop if there was a syntax error.
    if(hadError) return;

    AstPrinter printer;
    std::cout << printer.print(*expression) << std::endl;
}

void Lox::error(int line, string message) {
    report(line, "", message);
}

void Lox::report(int line, string where, string message) {
    cout << "[line " << line << "] Error" + where + ": " + message;
    hadError = true;
}

static void error(Token token, std::string message){
    if(token.type == TokenType::END_OF_FILE) {
        Lox::report(token.line, " at end", message);
    }
    else {
        Lox::report(token.line, " at '" + token.lexeme + "'", message);
    }
}
