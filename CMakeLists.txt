project(cpplox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpplox src/main.cpp 
    src/Runtime/Runtime.cpp
    src/Lexer/Lexer.cpp
    src/Token/Token.cpp
    src/Parser/Parser.cpp
    src/AstPrinter/AstPrinter.cpp
    src/Interpreter/Interpreter.cpp
    src/Semantic/Environment.cpp
)
