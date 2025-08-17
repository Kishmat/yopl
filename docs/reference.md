This section describes how YOPL source code is processed and what makes up the language.
## Compilation Phases

1. **Preprocessing**
    + Scans the source code
    + Handles `include` statement
    + Each file is included **only once**,even if requested multiple times.
2. **Lexing**
    + Source code is converted into tokens.
    + Examples:
        + Keywords: let, return, function
        + Identifiers: num, print
        + Symbols: { } ( ) ;
        + Literals: numbers, strings
3. **Parsing**
    + Tokens are parsed into an Abstract Syntax Tree (AST).
    + AST separates code into two core constructs: **statements** and **expressions**.
4. **Execution (Visitor Phase)**
    + Statements are executed.
    + Expressions are evaluated only when a statement asks for them.

## Language Core
YOPL programs are made of two components:
1. **Statements** → things you do (execute).
2. **Expressions** → things you calculate (evaluate).

<h3>Example: Variable Definition</h3>

```
let num = 10;
let a = (5 - 2) * num;
```

+ `let num = 10`; → Variable definition statement.
+ Value `10` → Expression.
+ `(5 - 2) * num` → Expression.

<h3>Example: Function Call</h3>

```
print(5, 6, 7);
```
+ `print(5, 6, 7);` → Function call statement.
+ Arguments `5`, `6`, `7` → Expressions.

**So basically:**
1. Statements run.
2. Expressions calculate.
3. The interpreter walks through the AST, asking expressions for their values whenever a statement needs them.

## YOPL Reference

This is not a tutorial. This is the *bare minimum description* of what YOPL has.  
If you want hand-holding, wrong place — YOPL’s motto is in the name: *You’re On Your Own*.  
---

### Preprocessor

Runs before the interpreter even thinks about your code.  
- No semicolons allowed here.  
- Must end with a newline.  
- File is only included once (so no infinite recursion of `include` nightmares).  

#### `include`
```yopl
include "other.yopl"
// other stuffs
```
?> Shoves the contents of another file into this one. That’s it.

### Variables
#### `let`
```
let num = 42;
let name = "yopl";
```

Declares a variable. They can be reassigned later;


### Functions
#### Define
```
function greet(who) {
    print("hello", who);
}
```
#### Call
```
greet("world");
```
#### return
```
function square(x) {
    return x * x;
}
```
Bails out of the function and throws a value back.

### Control Flow
#### `if` / `elseif` / `else`
```
if (x > 10) {
    print("big");
} elseif (x == 10) {
    print("exactly ten");
} else {
    print("small");
}
```
Classic branching. Except with `elseif` instead of `else if`. Don’t argue.

### Loops
#### `while`
```
let i = 0;
while (i < 3) {
    print(i);
    i = i + 1;
}
```

#### `for`
**Range-based. Think math, not C.**
```
for (i = 0 to 10) {
    print(i);
}
```
#### `continue`
Skips the rest of the loop body and keeps looping.

#### `break`
Exits the loop entirely.

### Modules
#### `use`
```
use("math");
```
Loads a module so you can call its functions.\
**Yes, modules are just functions you didn’t write.**

### Expressions
Everything that produces a value is an expression:
+ Literals: `10`, `"text"`
+ Arithmetic: `(5 - 2) * num`
+ Function calls: `print("hi")`
+ Variable access: `x`