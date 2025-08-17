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
    if(isReference)
    {
        Value* ref_value = Env::getVariablePtr(variable_value->printString());
        Env::defineVariable(variable_name,ref_value);
    }else{
        Env::defineVariable(variable_name,variable_value->getValue());
    }
}
void AST_var_assign::execute(){
    Value v = Env::getVariable(variable_name);
    if(v.type == Value::Type::REFERENCE)
    {
        *v.as_reference() = variable_value->getValue();
    }else if(v.type == Value::Type::ARRAY){
        Array* arr = v.as_array();
        int i = index->getValue().as_number().as_int();
        if(i < (int)arr->size)
        {
            arr->elements[i] = new Value(variable_value->getValue());
        }else{
            std::cout << "ERROR HERE" << std::endl;
            exit(-1);
        }
    }else{
        Env::assignVariable(variable_name,variable_value->getValue());
    }
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
    for(auto ref : isReferenced){
        func.isReferrenced.push_back(ref);
    }
    for(auto& stmt : function_body){
        func.body.push_back(stmt);
    }
    Env::functionTable[function_name] = func;
}
void AST_return_statement::execute(){
    throw ReturnException(return_value->getValue());
}
void AST_break_statement::execute(){
    throw BreakException();
}
void AST_continue_statement::execute(){
    throw ContinueException();
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
        bool elseif_handeled = false;
        for(auto& elseif_node : elseif_body){
            Value elseif_value;
            if(elseif_node->condition)
            {
                elseif_value = elseif_node->condition->getValue();
            }
            if(elseif_value.as_condition())
            {
                if(!elseif_node->if_body.empty())
                {
                    for(auto& stmt : elseif_node->if_body)
                    {
                        stmt->execute();
                    }
                }
                elseif_handeled = true;
                break;
            }
        }
        if(!elseif_handeled && !else_body.empty()){
            for(auto& stmt : else_body){
                stmt->execute();
            }
        }
    }
}
void AST_for_stmt::execute(){
    Value s = start->getValue();
    Value e = end->getValue();
    if(s.type == e.type && s.type == Value::Type::NUMBER){
        Env::pushScope();
        Env::defineVariable(idName,s);
        int stat = (int)s.as_number().getValue();
        int endd = (int)e.as_number().getValue();
        
            for(int i = stat; i <= endd; i++){
                Env::assignVariable(idName,Value(i));
                bool should_continue = false;
                for(auto& stmt : body){
                    try{
                        stmt->execute();
                    }catch(const BreakException&){
                        return;
                    }catch(const ContinueException&){
                        should_continue = true;
                        break;
                    }
                }
                if(should_continue) continue;
            }
        Env::popScope();
    }else{
        std::cout << "For loop expects start and end to be a number" << std::endl;
        exit(-1);
    }
}
void AST_while_stmt::execute() {
    Value con;
    if (condition) {
        con = condition->getValue();
        
        while (con.as_bool()) {
            try {
                for (auto& stmt : body) {
                    stmt->execute();  // Execute the statement
                }
            } catch (const BreakException&) {
                return;  // Break the while loop and exit the function
            } catch (const ContinueException&) {
                con = condition->getValue();  // Re-check the condition before continuing
                continue;  // Skip to the next iteration of the while loop
            }
            
            con = condition->getValue();
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
                else if(function_name == "use")
                    function_use(function_args);
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
Value AST_array::getValue(){
    std::vector<Value*> ele;
    for(auto& element : elements)
    {
        ele.push_back(new Value(element->getValue()));
    }
    return new Array(ele);
}

Value AST_property_call::getValue(){
    Value val = Env::getVariable(object);
    if(val.type == Value::Type::ARRAY)
    {
        Array* arr = val.as_array();
        if(isFunction)
        {
            if(propertyName == "push")
            {
                if(args.size() != 1)
                {
                    std::cout << "Array.push() expects a 1 argument but given " << args.size() << std::endl;
                    exit(-1);
                }
                arr->push(new Value(args[0]->getValue()));
            }else if(propertyName == "pop")
            {
                arr->pop();
            }else if(propertyName == "empty")
            {
                return Value(arr->empty());
            }else{
                std::cout << "No function " << propertyName << " exists for type Array" << std::endl;
                exit(-1);
            }
        }else{
            if(propertyName == "size")
                return Value((int)arr->size);
            else{
                std::cout << "No property " << propertyName << " exists for type Array" << std::endl;
                exit(-1);
            }
        }
    }else{
        std::cout << "Property " << propertyName << " not found for " << object << std::endl;
        exit(-1);
    }
    return Value();
}
Value AST_array_access::getValue(){
    Array* arr = Env::getVariable(array_name).as_array();
    size_t i = index->getValue().as_number().as_int();
    return *arr->at(i);
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

void __print(Value& val){
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
            case Value::Type::REFERENCE:{
                __print(*val.as_reference());
                break;
            }
            case Value::Type::ARRAY:{
                Array* ar = val.as_array();
                std::cout << "Array(" << ar->size << ") => {";
                for(size_t i=0;i<ar->size;i++){
                    __print(*ar->elements[i]);
                    if(i+1 != ar->size)
                        std::cout << ", ";
                }
                std::cout << "}";
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
            default:{
                break;
            }
        }
}

void function_print(std::vector<Expr*>& arguments){
    for(auto* arg : arguments)
    {
        Value val = arg->getValue();
        __print(val);
    }
    std::cout << "\n";
}
void function_use(std::vector<Expr*>& arguments){
    if(arguments.size() != 1){
        std::cout << "Use function expects only 1 argument" << std::endl;
        exit(-1);
    }
    Value val = arguments[0]->getValue();
    if(val.type != Value::Type::STRING){
        std::cout << "Module name must be of string type" << std::endl;
        exit(-1);
    }
    std::string file = val.as_string();
    Env::includeModule(file);
}

std::string __type_of(Value& val){
    Value::Type type = val.type;
    switch (type)
    {
        case Value::Type::NUMBER: return "Number";
        case Value::Type::STRING: return "String";
        case Value::Type::BOOL: return "Boolean";
        case Value::Type::ARRAY: return "Array";
        case Value::Type::REFERENCE: return __type_of(*val.as_reference());
        default: return "NULL";
    }
    return "NULL";
}
std::string function_typeof(std::vector<Expr*>& arguments){
    if(arguments.size() != 1){
        std::cout << "Include function expects only 1 argument" << std::endl;
        exit(-1);
    }
    Value val = arguments[0]->getValue();
    return __type_of(val);
}

Value visit_user_function(AST_func_call* function){
    UserFunction fn = Env::functionTable[function->function_name];
    if(fn.parameters.size() != function->function_args.size()){
        std::cout << "Function" << function->function_name << " expects " << fn.parameters.size() << " arguments but got " << function->function_args.size() << "\n";
        exit(-1);
    }
    Env::pushScope();
    for(size_t i = 0; i < fn.parameters.size(); i++){
        if(fn.isReferrenced[i])
        {
            Value* ref_value = Env::getVariablePtr(function->function_args[i]->printString());
            Env::defineVariable(fn.parameters[i],ref_value);
        }else{
            Env::defineVariable(fn.parameters[i],function->function_args[i]->getValue());
        }
    }
    for(auto& stmt : fn.body)
    {
        stmt->execute();
    }
    Env::popScope();
    return Value();
}
NativeFunction* getFunction(const std::string& name) {
    auto it = Env::modules.find(name);
    if (it != Env::modules.end()) {
        return &it->second;
    }
    return nullptr;
}
Value visit_moduled_function(AST_func_call* function) {
    std::vector<Value> args;
    for (auto& arg : function->function_args) {
        args.push_back(arg->getValue());
    }

    NativeFunction* func = getFunction(function->function_name);
    if (!func) {
        std::cerr << "Function '" << function->function_name << "' not found.\n";
        return Value(); // or throw an error
    }
    Value result = (*func)(args);
    if(result.type == Value::Type::ERR){
        std::cerr << "\n[Module Error] " << result.as_string() << std::endl;
        return Value();
    }
    return result;
}