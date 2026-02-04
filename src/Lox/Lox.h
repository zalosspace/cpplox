#pragma once

#ifndef LOX_H
#define LOX_H

#include <string>
#include "../Token/Token.h"
#include "../Interpreter/RuntimeError.h"
#include "../Interpreter/Interpreter.h"

class Lox{
public:
    static bool hadError;
    static bool hadRuntimeError;
    static Interpreter interpreter;

    static void run(const std::string& source);
    static void runFile(const std::string& path);
    static void runPrompt();

    static void error(int line, std::string message); 
    static void error(Token token, std::string message);
    static void runtimeError(const RuntimeError& error);
    static void report(int line, std::string where, std::string message); 
};

#endif
