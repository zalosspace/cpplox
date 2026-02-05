#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

#include "Lox.h"
#include "../Scanner/Scanner.h"
#include "../Parser/Parser.h"
#include "../AstPrinter/AstPrinter.h"

bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;
Interpreter Lox::interpreter;

void Lox::runPrompt(){
    while(true){
        std::string line;
        std::cout << "> ";

        // Take Input
        if(!getline(std::cin, line)) break;
        // Run Command
        Lox::run(line);

        // New line = New start
        hadError=false;
    }
}

void Lox::runFile(const std::string& path){
    // File input stream
    std::ifstream file(path);

    // Handle file read failure
    if(!file.is_open()){
        std::cerr << "Could not open file: " << path << std::endl;
        exit(74); // custom exit error code (file read failure)
    }

    // Read entire file into buffer
    std::stringstream buffer;
    buffer << file.rdbuf(); 
    Lox::run(buffer.str());

    // Exit If Error Occurs Anywhere 
    if(hadError) exit(65);
    if(hadRuntimeError) exit(70);
}

void Lox::run(const std::string& source){
    // Initialize the scanner
    Scanner scanner(source);

    std::vector<Token> tokens = scanner.scanTokens();
    // Print all the tokens
    for(const Token& token: tokens){
        std::cout << token << std::endl;
    }
    
    // Initialize the parser
    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
    if (statements.empty()) {
        std::cerr << "Parser returned nullptr!" << std::endl;
        return;
    }

    // Stop if there was a syntax error.
    if(hadError) return;

    // AstPrinter printer;
    // std::cout << printer.print(statements) << "\n";

    // Interpret
    interpreter.interpret(statements);
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::error(Token token, std::string message){
    if(token.type == TokenType::END_OF_FILE) {
        Lox::report(token.line, " at end", message);
    }
    else {
        Lox::report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void Lox::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message;
    hadError = true;
}

void Lox::runtimeError(const RuntimeError& error) {
    std::cerr << error.what()
              << "\n[line " << error.token.line << "]\n";
    hadRuntimeError = true;
}

