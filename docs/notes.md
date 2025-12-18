# Rules for grammars
## Keywords
- Productions: Set of rules for grammar.
- Derivations: Strings created by following productions.
- Terminal: A letter from the grammar's alphabet. We can think of
it like a literal value, lexeme. For example: if, 42069 
- Nonterminal: Named reference to another productions.

## Syntax
- Each rule is a name
- Followed by an arrow (→)
- Followed by a sequence of symbols
- Finally ending with a semicolon (;)
- Terminals are quoted strings
- Nonterminals are lowercase words

```
breakfast  → protein "with" breakfast "on the side" ;
breakfast  → protein ;
breakfast  → bread ;

protein    → crispiness "crispy" "bacon" ;
protein    → "sausage" ;
protein    → cooked "eggs" ;

crispiness → "really" ;
crispiness → "really" crispiness ;

cooked     → "scrambled" ;
cooked     → "poached" ;
cooked     → "fried" ;

bread      → "toast" ;
bread      → "biscuits" ;
bread      → "English muffin" ;
```

## Enhancing our notation
- Instead of repeating the rule name each time we want to add another production 
for it, we’ll allow a series of productions separated by a pipe `|`.
- Further, we’ll allow parentheses `( | )` for grouping and then allow `|` within that to 
select one from a series of options within the middle of a production.
- We use a postfix `*` to allow the previous symbol or group to be repeated 
zero or more times.
- A postfix `+` is similar, but requires the preceding production to 
appear at least once.
- A postfix `?` is for an optional production. The thing before it can appear 
zero or one time, but not more.

```
breakfast → protein ( "with" breakfast "on the side" )?
          | bread ;

protein   → "really"+ "crispy" "bacon"
          | "sausage"
          | ( "scrambled" | "poached" | "fried" ) "eggs" ;

bread     → "toast" | "biscuits" | "English muffin" ;
```
