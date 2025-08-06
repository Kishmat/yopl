#include "environment.h"
#include<sstream>
#include<fstream>

namespace Env{
    // interpreter
    std::string m_source;
    size_t source_length;
    std::vector<Token> m_tokens;
    std::vector<Stmt*> m_nodes;
    const std::unordered_set<std::string_view> keywords = {
        "let","function","return","if","else"
    };
    const std::unordered_set<std::string_view> functions = {
        "print","exit","typeof","input","include"
    };
    void init(){
        registerModule("string",{
        {
            "strlen", [](std::vector<Value> args) -> Value{
                if(args.size() == 1 && args[0].type == Value::Type::STRING)
                {
                    return Value((int)args[0].as_string().length());
                }
                return Value();
            }
        },
    });
    }
    void loadSource(std::string& fileName){
        std::ifstream file(fileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        m_source = buffer.str();
        source_length = m_source.size();
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
    void assignVariable(const std::string& name, Value val){
        for(auto it = scopes.rbegin(); it != scopes.rend(); ++it){
            if(it->count(name)){
                (*it)[name] = val;
                return;
            }
        }
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
    std::unordered_map<std::string, NativeFunctionMap> modules;
    std::vector<std::string> included_modules;

    void includeFile(std::string& name){
        if(modules.count(name))
        {
            if(std::find(included_modules.begin(),included_modules.end(),name) == included_modules.end())
            {
                included_modules.push_back(name);
            }
        }else{
            std::cout << "No any file or module named " << name << " found!" << std::endl;
            exit(-1);
        }
    }
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
            for (const std::string& mod : included_modules) {
                if (modules.count(mod) && modules[mod].count(name)) {
                    return true;
                }
            }
        }
        return isFunctionUserDefined(name);
    }
}

Value Env::Evaluate(AST_binary_expression* expression){
    Value left = expression->left->getValue();
    Value right = expression->right->getValue();
    std::string op = expression->op;

    // mathematics
    if(op == "+") return left+right;
    else if(op == "-") return left-right;
    else if(op == "*") return left*right;
    else if(op == "/") return left / right;
    else if(op == "%") return left%right;

    // comparisions
    else if(op == ">") return Value(left > right);
    else if(op == "<") return Value(left < right);
    else if(op == ">=") return Value(left >= right);
    else if(op == "<=") return Value(left <= right);
    else if(op == "==") return Value(left == right);
    else if(op == "!=") return Value(left != right);
    else{
        std::cout << "Unknown operator: " << op << std::endl;
        exit(-1);
    }
}

void Env::registerModule(std::string name, std::unordered_map<std::string,NativeFunction> functions){
    modules[name] = functions;
}