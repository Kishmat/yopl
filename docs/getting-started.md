# Getting Started with YOPL
## Installation

1. Download yopl.exe from the release section… or the code.
2. Put it in a folder you can somehow remember.\
`Optional: Install the VSCode extension for YOPL.It does syntax highlighting and autocomplete.Totally optional. Totally helpful. Maybe.`
3. If using VSCode, open a .yopl file and press F5 or Ctrl + Shift + R to run it.
Otherwise, open a terminal in the folder and type:
```console
yopl.exe your-file.yopl
```

?> **Pray for it to work.**

## Syntax Familiarity
Here’s a quick peek at what YOPL can roughly do.

### Variables
Variables are **dynamically typed**, and currently only 4-5 datatypes are supported i.e\
`Number, String, Boolean, Reference, Array` you get the idea.
```
let null_value; // null
let name = "YOPL"; // string
let age = 5; // number
let state = true; // boolean
let marks = 50.5; // number (float)
let &ref = name; // reference is denoted by & sign ahead of identifier
// for nerds reference basically means there are two names for a single variable
let marks = [50,60,70]; // array
```
### Functions
```
function greet(name){
    print("Hello " + name);
}
```
Yep, you can define functions. They will most probably work.
#### Return
You can even return values
```
function add(n1,n2){
    return n1+n2;
}
let sum = add(5,6);

```
#### Argument Passing
You can pass variables by you know usual stuffs by value or by reference
- By Value

```
let a = 5; // var def
function(a){
    a++; // a = a + 1
    print("Inside : ",a);
}
print("Outside : ", a);

// result
Inside : 6
Outside : 6
```
- By Reference

```
let a = 5; // var def
function(&a){ // remember above? i mentioed & symbol before identifier makes it a reference
    a++; // a = a + 1
    print("Inside : ",a);
}
print("Outside : ", a);

// result
Inside : 6
Outside : 6
```

### Conditionals
```
if(age < 12){
    print("Child");
}elseif(age > 12 && age < 18){
    print("Still Child");
}else{
    print("GROWN");
}
```
- If,elseif,else, comparisons, basic operators: > < >= <= == != + - * / && || ++ -- supported.
- Variables are true when `Number > 0`,`String not empty`,`Boolean is True`,`Array not empty` 
**That’s mostly it.**

## Built-In Functions
Believe it or not YOPL can print and input and stuff like that.
- ### print
    - prints stuff to the console
    - multiple arguments seperated by comma
    - returns Null
    ```
        print("hello", " world", " or ", "hello"+" world");
    ```
- ### input
    - takes user input from console
    - no arguments
    - returns variable value
    ```
        print("Enter a number : ");
        let num = input();
        // do something
    ```
- ### typeof
    - returns the type of data
    - single argument whose type is to be determinded
    - returns string
    ```
        print("Enter a number : ");
        let num = input();
        if( typeof(num) != "Number" )
        {
            print("Please enter a valid number");
        }else{
            // do something
        }
    ```
- ### exit
    - exit the code
    - no argument
    ```
        let risky = false;
        // something
        if(risky){
            print("Something risky! Program will quit");
            exit();
        }
    ```
- ### use
    - Used to include a module into the source file and use it's components.
    - Described under [Working with Modules](modules.md)

# First Program
Let’s write the classic “Hello, user” program.
```
print("Enter your name: ");
let name = input();
if(typeof(name) != "String"){
    print("Please input a valid name");
    exit(); // stops the program
}
print("Hello, " + name);
```
- Enter your name. It might work.

If it doesn’t… well, see the [error messages section](?id=error-messages-or-lack-thereof) above.