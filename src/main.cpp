#include "Runtime/Runtime.h"
#include <iostream>

int main(int argc, char *argv[]){
    // Handle 2 Arguments:
    // 1. cpplox (interpreter)
    // 2. script (path of script to run)
    if (argc > 2) {
        std::cerr << "Usage: cpplox [script]\n";
        return 64; // exit with error
    } 
    // Process file if path exist
    else if (argc == 2) { 
        return Runtime::processFile(argv[1]);
    }
    // Launch Read-Eval-Print Loop (REPL) 
    else {
        std::cout << "Welcome To Cpplox: A Tree-Walk Interpreter" << std::endl;
        return Runtime::launchREPL(); 
    }
}
