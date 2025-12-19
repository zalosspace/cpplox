#pragma once

#ifndef LOX_H
#define LOX_H

#include <string>
#include "../Token/Token.h"

class Lox{
public:
    static bool hadError;

    static void runPrompt();

    static void runFile(const std::string& path);

    static void run(std::string source);

    static void error(int line, std::string message); 

    static void report(int line, std::string where, std::string message); 

    static void error(Token token, std::string message);
};

#endif
