#pragma once

#include "../env/environment.h"

class Visitor{
public:
    void visit();
private:
};

Value visit_user_function(AST_func_call* function);
Value visit_moduled_function(AST_func_call* function);

void function_print(std::vector<Expr*>& arguments); 
void function_include(std::vector<Expr*>& arguments);
std::string function_typeof(std::vector<Expr*>& arguments);