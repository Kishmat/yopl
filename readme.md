# YOPL - You're On Your Own Programming Language

Welcome to **YOPL**, the language where you're on your own – literally.
# How to Use
- Download the `yopl.exe` file from release section or directly from the code and move it to a new folder where you remember.
- Create a new file with extension `.yopl` in the same folder as that file I told you to paste.
- In VsCode search for yopl in the extensions tab and install it. Totally optional but it has syntax highlighting and auto complete, how cool is that 😁
- If in VsCode and a `.yopl` file is open press `F5` or `Ctrl + Shift + R` to run the code. Make sure that the yopl.exe file is in the same folder as that file.
- If not VsCode then open command prompt in that folder and type
```bash
yopl.exe your-file.yopl
```

- Pray for it to work but if it doesn't then below is the The Documentation for it

# the The Documentation
## Sample Example
```javascript
print("Enter your name : ");
let name = input();
if(typeof(name) != "String"){
  print("Please input a valid name");
  exit(); // exits the program
}
let greet = "Hello! " + name; // string concats
print(greet);
```
## Other stuffs
- if and else block
- own function creation
```javascript
function even_odd(num){
  if(num%2 == 0){
    return true;
  }
  return false;
}
if(even_odd(5))
{
  print("It is even");
}else{
  print("It is Odd");
}
```
- other operators supported are `>`,`<`,`>=`,'`<=`,`==`,`!=`,`+`,`-`,`*`,`/`

Rest is upto you!

