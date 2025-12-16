#include "Lox/Lox.h"
#include <iostream>

int main(int argc, char* argv[]){
    std::cout << "Welcome To Cpplox: A Tree-Walk Interpreter" << std::endl;

    // Handle 2 Arguments:
    // 1. cpplox (interpreter)
    // 2. script (name of the script to run)
    if (argc > 2) {
        std::cout << "Usage: cpplox [script]";
        return 64; // exit with error
    } 
    // Run file if path exist
    else if (argc == 2) { 
        Lox::runFile(argv[1]);
    }
    // Run Interactive Prompt (REPL) 
    else {
        Lox::runPrompt(); 
    }

    return 0;
}


