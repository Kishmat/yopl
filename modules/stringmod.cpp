#include "../env/environment.h"
#include<algorithm>

extern "C" __declspec(dllexport) 
void registerModule(RegisterFunctionPtr regFn){
    regFn("strlen",[](std::vector<Value> args) -> Value{
        if (args.empty() || args[0].type != Value::Type::STRING) return Value();
        return Value((int)args[0].as_string().size());
    });
    regFn("strtoupper",[](std::vector<Value> args) -> Value{
        if(args.size() != 1){
            std::cout << "strtoupper function expects 1 argument but given " << args.size() << std::endl;
            exit(-1);
        }
        std::string str = args[0].as_string();
        std::transform(str.begin(),str.end(),str.begin(),::toupper);
        return Value(str);
    });
}