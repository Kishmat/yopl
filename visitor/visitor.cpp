#include "visitor.h"

void Visitor::visit(){
    try{
        for(auto* node : Env::m_nodes){
            node->execute();
        }
    }catch(const ReturnException& ret){
        std::cout << "Return called on a non-function block!" << std::endl;
        exit(-1);
    }
}

void AST_var_def::execute(){
    Env::defineVariable(variable_name,variable_value->getValue());
}
void AST_var_assign::execute(){
    Env::assignVariable(variable_name,variable_value->getValue());
}

void AST_func_def::execute(){
    if(Env::functionExists(function_name))
    {
        std::cout << "Function " << function_name << " already defined once" << std::endl;
        exit(-1); 
    }
    UserFunction func;
    for(auto& parm : function_params){
        func.parameters.push_back(parm);
    }
    for(auto& stmt : function_body){
        func.body.push_back(stmt);
    }
    Env::functionTable[function_name] = func;
}
void AST_return_statement::execute(){
    throw ReturnException(return_value->getValue());
}
void AST_if_statement::execute(){
    Value val;
    if(condition)
        val = condition->getValue();
    if(val.as_condition())
    {
        if(!if_body.empty()){
            for(auto& stmt : if_body){
                stmt->execute();
            }
        }
    }else{
        if(!else_body.empty()){
            for(auto& stmt : else_body){
                stmt->execute();
            }
        }
    }
}
void AST_expr_stmt::execute(){
    expression->getValue();
}

Value AST_func_call::getValue(){
    try{
        if(Env::functionExists(function_name))
        {
            if(Env::isFunctionBuiltIn(function_name))
            {
                if(function_name == "input")
                    return function_input();
                else if(function_name == "print")
                    function_print(function_args);
                else if(function_name == "include")
                    function_include(function_args);
                else if(function_name == "exit")
                    exit(1);
                else if(function_name == "typeof")
                {
                    std::string test = function_typeof(function_args);
                    return Value(test);
                }
            }else{
                if(Env::isFunctionUserDefined(function_name))
                    return visit_user_function(this);
                else
                    return visit_moduled_function(this);
            }
        }else{
            std::cout << "Undefined function " << function_name << std::endl;
            exit(-1);
        }
    } catch(const ReturnException& ret){
        return ret.value;
    }
    return Value();
}
Value AST_identifier::getValue(){
    return Env::getVariable(identifier_name);

}
Value AST_literal::getValue(){
    return literal_value;
}
Value AST_binary_expression::getValue(){
    return Env::Evaluate(this);
}
Value AST_unary_expression::getValue() {
    Value val = expr->getValue();

    if (op == "-") {
        if (val.type != Value::Type::NUMBER) {
            std::cerr << "Unary '-' applied to non-number: " << val.toString() << std::endl;
            exit(1);
        }

        const Number& num = val.as_number();

        if (num.type == Number::Type::INTEGER) {
            return Value(-num.as_int());
        } else if (num.type == Number::Type::DECIMAL) {
            return Value(-num.as_double());
        } else {
            std::cerr << "Invalid number type in unary '-': " << val.toString() << std::endl;
            exit(1);
        }
    }

    std::cerr << "Unsupported unary operator: " << op << std::endl;
    exit(1);
}


void function_print(std::vector<Expr*>& arguments){
    for(auto* arg : arguments)
    {
        Value val = arg->getValue();
        switch (val.type)
        {
            case Value::Type::NUMBER:{
                Number num = val.as_number();
                if(num.type == Number::Type::INTEGER)
                {
                    std::cout << num.as_int();
                }else{
                    std::cout << num.as_double();
                }
                break;
            }
            case Value::Type::STRING:{
                std::cout << val.as_string();
                break;
            }
            case Value::Type::BOOL:{
                bool state = val.as_bool();
                if(state)
                    std::cout << "true";
                else
                    std::cout << "false";
                break;
            }
            case Value::Type::NILL:{
                std::cout << "NULL";
                break;
            }
        }
    }
    std::cout << "\n";
}
void function_include(std::vector<Expr*>& arguments){
    if(arguments.size() != 1){
        std::cout << "Include function expects only 1 argument" << std::endl;
        exit(-1);
    }
    Value val = arguments[0]->getValue();
    if(val.type != Value::Type::STRING){
        std::cout << "include name must be of string type" << std::endl;
        exit(-1);
    }
    std::string file = val.as_string();
    Env::includeFile(file);
}
std::string function_typeof(std::vector<Expr*>& arguments){
    if(arguments.size() != 1){
        std::cout << "Include function expects only 1 argument" << std::endl;
        exit(-1);
    }
    Value::Type type = arguments[0]->getValue().type;
    switch (type)
    {
        case Value::Type::NUMBER: return "Number";
        case Value::Type::STRING: return "String";
        case Value::Type::BOOL: return "Boolean";
        default: return "NULL";
    }
    return "NULL";
}

Value visit_user_function(AST_func_call* function){
    UserFunction fn = Env::functionTable[function->function_name];
    if(fn.parameters.size() != function->function_args.size()){
        std::cout << "Function" << function->function_name << " expects " << fn.parameters.size() << " arguments but got " << function->function_args.size() << "\n";
        exit(-1);
    }
    Env::pushScope();
    for(size_t i = 0; i < fn.parameters.size(); i++){
        Env::defineVariable(fn.parameters[i],function->function_args[i]->getValue());
    }
    for(auto& stmt : fn.body)
    {
        stmt->execute();
    }
    Env::popScope();
    return Value();
}
Value visit_moduled_function(AST_func_call* function){
    std::vector<Value> args;
    for (auto& arg : function->function_args) {
        args.push_back(arg->getValue());
    }
    for (const std::string& mod : Env::included_modules) {
        auto& funcs = Env::modules[mod];
        if (funcs.count(function->function_name)) {
            auto nativeFunc = funcs[function->function_name];
            return nativeFunc(args);
        }
    }
    return Value();
}