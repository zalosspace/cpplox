# Parser Expression Grammar
```
expression     → literal
               | unary
               | binary
               | grouping ;

literal        → NUMBER | STRING | "true" | "false" | "nil" ;
grouping       → "(" expression ")" ;
unary          → ( "-" | "!" ) expression ;
binary         → expression operator expression ;
operator       → "==" | "!=" | "<" | "<=" | ">" | ">="
               | "+"  | "-"  | "*" | "/" ;
```

## Note: Current Expression Grammar Can Lead To False Results
`6/3-1`
Two ways to interpret this using our current syntax:- 
- (6 / (3 - 1))
- ((6 / 3) - 1))

Both fits in our current grammar but both doesn't output the same result so,
we need a more sophisticated syntax.

## Upgrade Syntax
- __**Precedence**__: determines which operator is evaluated first in an expression 
containing a mixture of different operators. Precedence rules tell us that 
we evaluate the / before the - in the above example.
- __**Associativity**__: determines which operator is evaluated first in a series 
of the same operator. When an operator is **left-associative** 
(think “left-to-right”). This expression `5 - 3 - 1` would be evaluated to
`( 5 - 3 ) - 1` but if the operator is **right-associative** it would be 
equaluated accordingly for examle `a = b = c` -> `a = ( b = c)`.

### Our Rules
We'll use the same precedence rules as C, going from lowest to highest.

| Name       | Operators   | Associates |
|------------|------------|------------|
| Equality   | ==, !=     | Left       |
| Comparison | >, >=, <, <= | Left    |
| Term       | -, +       | Left       |
| Factor     | /, *       | Left       |
| Unary      | !, -       | Right      |

After applying these to our grammar we'll get our complete Expression Grammar:

```
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
                 | equality;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → "true" | "false" | "nil"
                 | NUMBER | STRING | 
                 | "(" expression ")"
                 | IDENTIFIER ;
```

## Parsing Technique
### Recursive Descent
It is the simplest way to build a parser. It is fast robust, and can 
support sophisticated error handling. GCC, V8 and many other heavyweight
production language implementations use recursive descent.

It is considered a _top-down_ parser because it starts from the top or 
outermost grammer rule (here expression) and works its way down into the 
nested subexpressions before reaching the leaves of the syntax tree.

## Truthiness and falsiness
In Lox false & nil are *flasey* and everything else is *truthy*.

## Statement and State
```
program        → declaration* EOF ;
declaration    → varDecl | statement ;

varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
statement      → exprStmt | printStmt | block ;
block          → "{" declaration* "}" ;

exprStmt       → expression ";" ;
printStmt      → "print" expression ";" ;
```
### Environments
It's a data structure which stores the bindings that associate variables
to value. Basically it holds variable and it's value.
