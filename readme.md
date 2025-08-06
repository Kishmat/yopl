# YOPL - You're On Your Own Programming Language

Welcome to **YOPL**, the language where you're on your own – literally.
# Simple Example
```bash
print("Enter your name : ");
let name = input();
if(typeof(name) != "String"){
  print("Please input a valid name");
  exit(); // exits the program
}
let greet = "Hello! " + name; // string concats
print(greet);
```
# Other stuffs
- if and else block
- own function creation
```bash
function even_odd(num){
  if(num%2 == 0){
    return true;
  }
  return false;
}
if(even_odd(5))
{
  print(It is even");
}else{
  print("It is Odd");
}
```
- other operators supported are `>`,`<`,`>=`,'`<=`,`==`,`!=`,`+`,`-`,`*`,`/`

Rest is upto you!

