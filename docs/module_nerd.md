# Writing a module (For Nerds)

Here’s the absolute barebones rundown — because obviously you’re smart enough to "figure out the rest".


## What is `module.h`
Think of `module.h` as your **box of sharp knives**. You could cut veggies with it, or you could cut your own fingers.\
Either way:

- **Data Types** (a.k.a. “things YOPL actually understands”):
    - `YOPL_TYPE_NUMBER` (yes, numbers, shocking)
    - `YOPL_TYPE_STRING` (words, woohoo)
    - `YOPL_TYPE_BOOL` (true or false, pick a side)
    - `YOPL_TYPE_NULL` (the void™)
    - `YOPL_TYPE_REFERENCE` (pointers to other values, for when you hate yourself)
    - `YOPL_TYPE_ARRAY` (a box of other boxes)
    - `YOPL_TYPE_ERROR` (because you will mess up)

- **Special Types:**
    - `YOPL_Value` – the magical container of literally everything. Numbers, strings, arrays, your hopes, your regrets.
    - `YOPL_Number` – can be `INTEGER` or `DECIMAL` because someone thought floats were too mainstream.
    - `YOPL_Array` – holds `YOPL_Value*` elements (yes, pointers, enjoy the segfaults).

- `setValue()`
    - Always assign value to a `YOPL_Value` using this function in order to prevent from getting a *headache*.

## Defining Functions
YOPL loves making you feel fancy, so you don’t just write a function.
You **DefineFunction**:

```
YOPL_DefineFunction(myFunc) {
    // argc = number of arguments passed while calling the function from yopl
    // argv = the actual arguments passed while calling the function from yopl
    // return a YOPL_Value, or the runtime will roast you
}
```
That’s it. You just magically get `argc` and `argv`. No rituals, no blood sacrifices (yet).

## The One Function That Matters
Every module **must** have a:
```
void registerModule(YOPL_RegisterFn regFn) {
    regFn("funcName", funcPointer);
}
```

Think of `registerModule` as your module’s **main()**, except instead of printing `"Hello World"`, you’re **selling your soul** by handing YOPL a bunch of functions.

## Compilation (a.k.a. Making the Ritual Complete)
Run this spell:
```sh
g++ -shared source.cpp -o module.yopl.mod
```
I know it would have been a **DLL** by default but make sure to rename it to `.yopl.mod` because YOPL likes cosplay.


!> **Important**:\
Your module must be **self-contained**. If you pull in external libraries, link them **statically**.\
No one wants your 30 DLL dependencies.

## Examples
1. Simple hello world example

`mymodule.cpp`
```cpp
#include"module.h" // yopl's module api
#include<iostream> // for std::cout

YOPL_DefineFunction(say_hello){ // say_hello is the name of definition
    std::cout << "Hello World from my module!" << std::endl;
    return YOPL_Value(); // returns null by default since YOPL asks for it
}

void registerModule(YOPL_RegisterFn regFn){ 
    // register your functions here
    regFn("say_hello",say_hello); // name of the function, name of the definition
}
```
```sh
g++ -shared mymodule.cpp -o mymodule.yopl.mod
```

`mycode.yopl`
```
use("mymodule"); // loads the function registration from mymodule.yopl.mod
say_hello(); // prints Hello World from my module!
```

2. Something actually useful

`string.cpp`
```cpp
#include"module.h" // yopl's module api
#include<string.h> // for strlen function

YOPL_DefineFunction(strlen){
    if(argc != 1)
        return YOPL_Error("strlen function requires 1 argument");
    if(argv[0].type != YOPL_TYPE_STRING)
        return YOPL_Error("strlen function expects a string as argument");
    
    YOPL_Value length; // define for length
    length.setValue(strlen(argv[0].string));
    return length;
}

void registerModule(YOPL_RegisterFn regFn){ 
    regFn("strlen",strlen);
}
```
```sh
g++ -shared string.cpp -o string.yopl.mod
```

`mycode.yopl`
```
use("string");
let length = strlen("Hello World");
print(length);
```