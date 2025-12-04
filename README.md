# Cpp Lox 
## Things To Know
### Lexemes
A lexemes is the raw piece of text your code contains, For Example:- 
- "int"
- "x"
- "="
- "69"
- ";"
   
### Tokens
A token is the label or category you language gives to that lexeme.
- "int" → KEYWORD
- "x" → IDENTIFIER
- "=" → ASSIGN_OP
- "69" → NUMBER_LITERAL
- ";" → SEMICOLON

## Frontend
### Scanning
Also known as lexing (lexical analysis), A scanner/lexer takes in the
linear stream of characters (lexemes) and chunks them together into 
a series of word (token).

### Parsing
A parser takes the flat sequence of tokens and build a tree structure 
often refered to as - parse tree or abstract syntax treee (AST).
Actually Parser's the friend/foe which reports syntax errors. 
![AST Image](./assets/ast.png")

### Static Analysis 
If the language is statically typed, this is when we type check.
Let's suppose we have an expression like a + b so we would check
where a and b are declared, then we also figure out their types.
Then if those types don’t support being added to each other,
we report a type error.

## MiddleEnd
### Intermediate Representation (IR)
IR is the in-between form between AST and machine code,
The compiler translates AST to IR so it can optimize.

### Optimization
Self Explanatory...And Most Language Hackers Spend Their Entire
Careers Here, squeezing every drop of performance.

## Backend
### Code Generation
After implementing all the optimization you can think of 
we generate code in primitive assembly-like instruction which
the CPU can run.

### Bytecode Translation
If the compiler produces bytecode then you have to compile that bytecode
again cause there's no chip that runs fuckin bytecode.

# Derivatives
## Single Pass Compiles
While some compiler reads the source code several time inorder
to create the AST, IR or Optimizations single pass compiler reads
source code once, from top to bottom, and generates output as it goes,
without ever rewinding or re-parsing.

Pascal & C were designed around this limitation because memory at that 
time was so precious that a compiler might not even be able to hold 
the entire source file.

## Tree-walk Interpreters
Some programming languages begin executing code right after parsing 
it to an AST (with maybe a bit of static analysis applied).

It's not widely used for general-purpose languages since it tends to be
slow.

## Transpilers
Transpiler as known as source-to-source compiler is a tool that 
takes code of one language and converts it into another language so that
developer don't have write a full fledged language.

## Just-In-Time Compilation
JIT compiles the code at runtime on the fly...
Alien Tech.





