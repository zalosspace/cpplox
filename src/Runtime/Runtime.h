#pragma once

#include <string>
#include "../Token/Token.h"
#include "../Interpreter/RuntimeError.h"
#include "../Interpreter/Interpreter.h"

class Runtime{
public:
    // Entry point
    static int processFile(const std::string& path);
    static int launchREPL();

    // Error reporting
    static void error(int line, const std::string message); 
    static void error(const Token& token, const std::string message);
    static void runtimeError(const RuntimeError& error);

private: 
    // State
    static bool s_hadError;
    static bool s_hadRuntimeError;
    static Interpreter s_interpreter;

    // Internal execution
    static void execute(const std::string& source);
    static void report(int line, const std::string where, const std::string message); 
};
