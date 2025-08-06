#pragma once
#include<string>
#include<vector>
#include<functional>
#include<unordered_set>
#include<unordered_map>
#include<string_view>

#include "../lexer/tokens.h"
#include "../parser/ast.h"

struct UserFunction{
    std::vector<std::string> parameters;
    std::vector<Stmt*> body;
};

struct ReturnException : public std::exception{
    Value value;
    explicit ReturnException(const Value& val) : value(val) {}
    const char* what() const noexcept override{
        return "Function returned";
    }
};
using NativeFunction = std::function<Value(std::vector<Value>)>;

namespace Env{
    // aliases
    using Scope = std::unordered_map<std::string, Value>;
    using FunctionMap = std::unordered_map<std::string, UserFunction>;
    using NativeFunctionMap = std::unordered_map<std::string, NativeFunction>;


    // interpreter
    extern std::string m_source;
    extern size_t source_length;
    extern std::vector<Token> m_tokens;
    extern std::vector<Stmt*> m_nodes;
    extern const std::unordered_set<std::string_view> keywords;
    extern const std::unordered_set<std::string_view> functions;

    void init();
    void loadSource(std::string& fileName);

    // variables
    extern std::vector<Scope> scopes;

    void pushScope();
    void popScope();
    void defineVariable(const std::string& name, Value val);
    void assignVariable(const std::string& name, Value val);
    Value getVariable(const std::string& name);


    // functions
    extern FunctionMap functionTable;
    extern std::unordered_map<std::string, NativeFunctionMap> modules;
    extern std::vector<std::string> included_modules;

    void includeFile(std::string& name);
    bool isFunctionUserDefined(std::string& name);
    bool isFunctionBuiltIn(std::string& function);
    bool functionExists(std::string& name);


    // helper
    Value Evaluate(AST_binary_expression* expression);
    void registerModule(std::string name, std::unordered_map<std::string,NativeFunction> functions);
}