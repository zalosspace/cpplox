#include "Runtime.h"
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
#include "../AstPrinter/AstPrinter.h"

#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

bool Runtime::s_hadError = false;
bool Runtime::s_hadRuntimeError = false;
Interpreter Runtime::s_interpreter;

int Runtime::launchREPL(){
    while(true){
        std::string line;
        std::cout << ">> ";

        // Take Input
        if(!getline(std::cin, line)) break;
        // Run Command
        Runtime::execute(line);

        // New line = New start
        s_hadError=false;
    }

    return 0;
}

int Runtime::processFile(const std::string& path){
    // File input stream
    std::ifstream file(path);

    // Handle file read failure
    if(!file.is_open()){
        std::cerr << "Could not open file: " << path << std::endl;
        return 74; // I/O error (can't open file)
    }

    // Read entire file into buffer
    std::stringstream buffer;
    buffer << file.rdbuf(); 
    Runtime::execute(buffer.str());
    
    if(s_hadError) 
        return 65; // Data format error (syntax/parse error)

    if(s_hadRuntimeError) 
        return 70; // Internal software error (runtime error)

    return 0; 
}

void Runtime::execute(const std::string& source){
    // Initialize the scanner
    Lexer lexer(source);

    std::vector<Token> tokens = lexer.scanTokens();
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
    if(s_hadError) return;

    // AstPrinter printer;
    // std::cout << printer.print(statements) << "\n";

    // Interpret
    s_interpreter.interpret(statements);
}

void Runtime::error(int line, std::string message) {
    report(line, "", message);
}

void Runtime::error(const Token& token, std::string message){
    if(token.type == TokenType::END_OF_FILE) {
        Runtime::report(token.line, " at end", message);
    }
    else {
        Runtime::report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void Runtime::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message;
    s_hadError = true;
}

void Runtime::runtimeError(const RuntimeError& error) {
    std::cerr << error.what()
              << "\n[line " << error.token.line << "]\n";
    s_hadRuntimeError = true;
}
