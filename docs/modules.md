# Working with Modules
## Need of Modules
YOPL by itself is… minimal.
Built-in functions? `print`, `input`, `typeof`, `exit`, and a few others. That’s it. Don’t ask for more.\
So what if you want something slightly useful? Like a **random number** from 1 to 10?\
It would look something like this:
```js
function random(start,end){
    // Logic… ahm. You’d have to add it yourself
}
```
Sure, you could add it as a built-in like `print`. Then you’d need to recompile, distribute the new binary, and pray it works.

## Modules
<h3>The lie</h3>

Modules are YOPL’s way of saying: *“Here, have some extra powers… without touching the core.”*
+ Keep the **math.yopl.mod** file relative to the source file.
```
use("math"); // magically loads math.yopl.mod
let ran = random(1,10);
print(ran);
```
+ **use("math")** handles the loading automatically.
+ The `random` function will return a **random number from 1 to 10** considering the module provides defination for it.
+ No need to recompile the whole interpreter.
**Functions work instantly. Wow.**

<h3>The truth</h3>

?> Even though it looks like a custom mdoule system with **.yopl.mod** extension, under the hood it’s just a **DLL** renamed as **.yopl.mod** 😂.\
Yes. You get to feel like a wizard loading custom binary modules while everyone else thinks it’s some cute file extension. ✨

?> **Modules:** \
YOPL’s **“cheat codes”** for people who want power without pain.

