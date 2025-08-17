#include<sstream>
#include<fstream>
#include<algorithm>
#include<windows.h>

#include "environment.h"
#include "../api/module.h"

namespace Env{
    // interpreter
    fs::path working_directory;
    std::string m_source;
    size_t source_length;
    std::vector<Token> m_tokens;
    std::vector<Stmt*> m_nodes;
    const std::unordered_set<std::string_view> keywords = {
        "let","function","return","if","else","for","while","to","elseif","break","continue"
    };
    const std::unordered_set<std::string_view> functions = {
        "print","exit","typeof","input","use"
    };

    std::unordered_set<fs::path> includedFiles;

    std::string getSourceCode(fs::path& fileName){
        std::ifstream file(fileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // variables
    std::vector<Scope> scopes = { {} };

    void pushScope(){
        scopes.push_back({});
    }
    void popScope(){
        scopes.pop_back();
    }
    void defineVariable(const std::string& name, Value val){
        if(scopes.back().count(name))
        {
            std::cout << "Variable " << name <<  " already defined in the current scope!" << std::endl;
            exit(-1);
        }
        scopes.back()[name] = val;
    }
    void assignVariablePtr(const std::string& name, Value val){
        for(auto it = scopes.rbegin(); it != scopes.rend(); ++it){
            if(it->count(name)){
                (*it)[name] = &val;
                return;
            }
        }
    }
    void assignVariable(const std::string& name, Value val){
        for(auto it = scopes.rbegin(); it != scopes.rend(); ++it){
            if(it->count(name)){
                (*it)[name] = val;
                return;
            }
        }
    }
    Value* getVariablePtr(const std::string& name){
        for(auto it = scopes.rbegin(); it != scopes.rend(); ++it){
            if(it->count(name)) return &(*it)[name];
        }
        return nullptr;
        std::cout << "Undefined variable: " << name << std::endl;
        exit(-1);
    }
    Value getVariable(const std::string& name){
        for(auto it = scopes.rbegin(); it != scopes.rend(); ++it){
            if(it->count(name)) return (*it)[name];
        }
        std::cout << "Undefined variable: " << name << std::endl;
        exit(-1);
    }


    // functions
    FunctionMap functionTable;
    NativeFunctionMap modules;

    bool isFunctionUserDefined(std::string& name){
        if(functionTable.count(name))
            return true;
        return false;
    }
    bool isFunctionBuiltIn(std::string& function){
        if(functions.count(function))
            return true;
        return false;
    }
    bool functionExists(std::string& name){
        if(functions.count(name))
            return true;
        else{
            if(modules.count(name))
                return true;
        }
        return isFunctionUserDefined(name);
    }
}

Value Env::Evaluate(AST_binary_expression* expression){
    Value left = expression->left->getValue();
    Value right;
    if(expression->right)
        right = expression->right->getValue();
    std::string op = expression->op;

    // mathematics
    if(op == "+") return left+right;
    else if(op == "-") return left-right;
    else if(op == "*") return left*right;
    else if(op == "/") return left / right;
    else if(op == "%") return left%right;

    else if(op == "++") return left + Value(1);
    else if(op == "--") return left - Value(1);

    // comparisions
    else if(op == ">") return Value(left > right);
    else if(op == "<") return Value(left < right);
    else if(op == ">=") return Value(left >= right);
    else if(op == "<=") return Value(left <= right);
    else if(op == "==") return Value(left == right);
    else if(op == "!=") return Value(left != right);
    else if (op == "&&") {
        bool left_bool = left.as_condition();
        bool right_bool = right.as_condition();
        return Value(left_bool && right_bool);
    }

    else if (op == "||") {
        bool left_bool = left.as_condition();
        bool right_bool = right.as_condition();
        std::cout << left.type << " " << right.type << std::endl;
        return Value(left_bool || right_bool);
    }
    else{
        std::cout << "Unknown operator: " << op << std::endl;
        exit(-1);
    }
}



void Env::includeModule(std::string& name){
    name = name + ".yopl.mod";
    fs::path fullPath = Env::working_directory / name;
    HMODULE handle = LoadLibraryA(fullPath.string().c_str());
    if(!handle){
        std::cerr << "Failed to load module: " << name << "\n";
        exit(1);
    }

    typedef void (*RegisterModuleFn)(YOPL_RegisterFn);
    RegisterModuleFn registerModule = (RegisterModuleFn)GetProcAddress(handle,"registerModule");
    if(!registerModule){
        std::cerr << "Invalid module file " << name << " because registerModule not found in it!" << std::endl;
        exit(1); 
    }
    registerModule([](const char* fnName, YOPL_NativeFn fn){
        NativeFunction wrapper = [fn](std::vector<Value> args) -> Value{
            std::vector<YOPL_Value> cArgs;
            cArgs.reserve(args.size());
            for(auto& arg : args)
                cArgs.push_back(convertToPublic(arg));
            YOPL_Value ret = fn((int)cArgs.size(),cArgs.data());
            return convertFromPublic(ret);
        };
        modules[fnName] = wrapper;
    });
}
