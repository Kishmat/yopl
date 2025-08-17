# Writing a module (For Non-Nerds)

So, you want to make YOPL more powerful by adding your own functions. Good choice.\
This guide will walk you through **how modules work**, step by step, without assuming you’re a C++ wizard.

## What Are Modules?

Modules are little **packs of extra powers** for YOPL.\
Instead of recompiling the whole interpreter every time you want `random()` or `strlen()`, you can just load a `.yopl.mod` file.\
It’s basically like giving YOPL a pocket knife for specific tasks, without letting it loose on the entire kitchen.
---

## The Magic Behind `module.h`
The `module.h` header is your **API** — it tells your C++ code how to talk to YOPL.\
- **Data Types** – the types YOPL actually understands:
    - `YOPL_TYPE_NUMBER` → numbers (integers or decimals)
    - `YOPL_TYPE_STRING` → text
    - `YOPL_TYPE_BOOL` → true/false
    - `YOPL_TYPE_NULL` → nothingness
    - `YOPL_TYPE_REFERENCE` → pointers to other values
    - `YOPL_TYPE_ARRAY` → a container of values
    - `YOPL_TYPE_ERROR` → when you mess up

- **Special Types**
    - `YOPL_Value` – the universal container. Every function must return this.
    - `YOPL_Number` – stores either an integer or decimal.
    - `YOPL_Array` – dynamic array of `YOPL_Value*` pointers.

- **Helpers**
    - `setValue()` → sets a value safely inside a `YOPL_Value`.
    - `YOPL_DefineFunction(name)` → creates a function for YOPL to call.
    - `YOPL_Error(msg)` → return an error to YOPL gracefully.

**Behind the scenes:** When you define a function using `YOPL_DefineFunction`, it’s actually just a C++ function with a fixed signature:
```cpp
YOPL_Value myFunc(int argc, YOPL_Value* argv);
```

- `argc` → number of arguments passed from YOPL.
- `argv` → an array of values (`YOPL_Value`) that YOPL sent to your function.

When YOPL calls your function, it evaluates the expressions in the script, packs them into `argv`, and lets your C++ code handle it. You then return a `YOPL_Value`, which YOPL uses as the result.

## The One Function You Must Have
Every module needs **exactly one entry point**:
```cpp
void registerModule(YOPL_RegisterFn regFn) {
    regFn("funcName", funcPointer);
}
```
- Think of `registerModule` as your module’s **main function**.
- `regFn` is a function pointer provided by YOPL so you can register every function you want the interpreter to know about.
- Only functions registered here will be visible in YOPL. Everything else in your `.cpp` file stays private.

**Behind the scenes:**
When YOPL loads your `.yopl.mod`, it calls `registerModule` and passes it the internal function `regFn`. This connects your C++ functions to the interpreter at runtime.
---
## Defining a Function
Use the provided macro:
```cpp
YOPL_DefineFunction(myFunc){
    // argc = number of arguments
    // argv = array of values passed
    // return a YOPL_Value
}
```
- Always return a `YOPL_Value`.
- You can return `YOPL_Error("message")` if something goes wrong.

?> Think of it as a small contract: YOPL calls you → you return a value → everyone is happy.

## Compilation
Compile your module as a **shared library**:
```sh
g++ -shared source.cpp -o module.yopl.mod
```
- On Windows, you can create a `.dll` and rename it to `.yopl.mod` (YOPL doesn’t judge).
- Make sure the module is **self-contained**. If you use external libraries, link them statically.
- This ensures that the `.mod` file works anywhere, without needing extra DLLs.



## Examples
1. ### Hello World Module
```cpp
#include "module.h"
#include <iostream>
YOPL_DefineFunction(say_hello){
    std::cout << "Hello World from my module!" << std::endl;
    return YOPL_Value(); // null by default
}
void registerModule(YOPL_RegisterFn regFn){
    regFn("say_hello", say_hello);
}
```
#### Compile:
```sh
g++ -shared mymodule.cpp -o mymodule.yopl.mod
```
#### Use in YOPL:
```
use("mymodule");
say_hello(); // prints "Hello World from my module!"
```

2. ### String Length Module
```cpp
#include "module.h"
#include <string.h>
YOPL_DefineFunction(strlen){
    if(argc != 1)
        return YOPL_Error("strlen requires 1 argument");
    if(argv[0].type != YOPL_TYPE_STRING)
        return YOPL_Error("strlen expects a string");

    YOPL_Value length;
    length.setValue(strlen(argv[0].string));
    return length;
}
void registerModule(YOPL_RegisterFn regFn){
    regFn("strlen", strlen);
}
```

#### Compile:
```sh
g++ -shared string.cpp -o string.yopl.mod
```

#### Use in YOPL:
```
use("string");
let len = strlen("Hello World");
print(len); // prints 11
```

## Behind the C++ Curtain
- Your module is a shared library (`.dll` or `.so`).
- `registerModule` is the entry point that connects your functions to YOPL.
- Each function defined with `YOPL_DefineFunction` is a standard C++ function, but YOPL handles packing/unpacking arguments for you.
- Think of it as YOPL being the “director” and your module functions being the “actors”. You act, YOPL collects the results, repeat.

**By now, you’re basically a module wizard.**

?> *"Not all heroes wear capes… some compile `.yopl.mod`s".* \
\- **Definitely not the Author**