#pragma once
#include<string>
#include<vector>
#include<iostream>
#include "../env/value.h"
struct Stmt
{
    virtual void print() = 0;
    virtual void execute() = 0;
};
struct Expr{
    virtual std::string printString() = 0;
    virtual Value getValue() = 0;
    // Value evaluate();
};

struct AST_var_def : public Stmt{
    std::string variable_name;
    Expr* variable_value = nullptr;
    bool isReference = false;

    void print() override{
        std::string text = "[VARIABLE_DEFINATION] => {\n";
        text += "\t[VARIABLE_NAME] => ";
        if(isReference) text += "&";
        text += variable_name + "\n";
        text += "\t[VARIABLE_VALUE] => " + variable_value->printString() + "\n";
        text += "\n}";
        std::cout << text << std::endl;
    }

    void execute() override;
};

struct AST_var_assign : public Stmt{
    std::string variable_name;
    Expr* variable_value = nullptr;

    void print() override{
        std::string text = "[VARIABLE_ASSIGN] => {\n";
        text += "\t[VARIABLE_NAME] => " + variable_name + "\n";
        text += "\t[VARIABLE_VALUE] => " + variable_value->printString() + "\n";
        text += "\n}";
        std::cout << text << std::endl;
    }

    void execute() override;
};
struct AST_func_def : public Stmt{
    std::string function_name;
    std::vector<std::string> function_params;
    std::vector<Stmt*> function_body;
    std::vector<bool> isReferenced;

    void print() override {
        std::cout << "[FUNCTION_DEFINITION] {\n";
        std::cout << "\tName: " << function_name << "\n";
        std::cout << "\tParameters: (";
        for (size_t i = 0; i < function_params.size(); ++i) {
            std::cout << function_params[i];
            if (i != function_params.size() - 1) std::cout << ", ";
        }
        std::cout << ")\n";
        std::cout << "\tBody:\n";
        for (Stmt* stmt : function_body) {
            std::cout << "\t\t";
            stmt->print();
        }
        std::cout << "}" << std::endl;
    }

    void execute() override;
};

struct AST_return_statement : public Stmt{
    Expr* return_value = nullptr;
    void print() override {
        std::cout << "[RETURN_STATEMENT] ";
        if (return_value) {
            std::cout << "Value: " << return_value->printString();
        } else {
            std::cout << "Value: null";
        }
        std::cout << std::endl;
    }

    void execute() override;
};
struct AST_break_statement : public Stmt{
    void print() override {
        std::cout << "[BREAK_STATEMENT] \n";
    }

    void execute() override;
};
struct AST_continue_statement : public Stmt{
    void print() override {
        std::cout << "[CONTINUE_STATEMENT] \n";
    }

    void execute() override;
};
struct AST_if_statement : public Stmt{
    Expr* condition = nullptr;
    std::vector<Stmt*> if_body;
    std::vector<Stmt*> else_body;
    std::vector<AST_if_statement*> elseif_body;

    void print() override {
        std::cout << "[IF_STATEMENT] {\n";
        std::cout << "\tCondition: " << (condition ? condition->printString() : "null") << "\n";
        std::cout << "\tIf Body:\n";
        for (Stmt* stmt : if_body) {
            std::cout << "\t\t";
            stmt->print();
        }
        if(!elseif_body.empty())
        {
            std::cout << "\tElseIf Body:\n";
            for(auto* if_stmt : elseif_body)
            {
                if_stmt->print();
            }
        }
        if (!else_body.empty()) {
            std::cout << "\tElse Body:\n";
            for (Stmt* stmt : else_body) {
                std::cout << "\t\t";
                stmt->print();
            }
        }
        std::cout << "}" << std::endl;
    }
    void execute() override;
};

struct AST_for_stmt : public Stmt{
    std::string idName;
    Expr* start = nullptr;
    Expr* end = nullptr;
    std::vector<Stmt*> body;

    
    void print() override {
        std::cout << "[FOR_LOOP] {\n";
        std::cout << "\tStart: " << (start ? start->printString() : "null") << "\n";
        std::cout << "\tEnd: " << (end ? end->printString() : "null") << "\n";
        std::cout << "\tBody:\n";
        for (Stmt* stmt : body) {
            std::cout << "\t\t";
            stmt->print();
        }
        std::cout << "}" << std::endl;
    }
    void execute() override;
};
struct AST_while_stmt : public Stmt{
    Expr* condition = nullptr;
    std::vector<Stmt*> body;
    void print() override {
        std::cout << "[WHILE_LOOP] {\n";
        std::cout << "\tCOndition: " << (condition ? condition->printString() : "null") << "\n";
        std::cout << "\tBody:\n";
        for (Stmt* stmt : body) {
            std::cout << "\t\t";
            stmt->print();
        }
        std::cout << "}" << std::endl;
    }
    void execute() override;
};
struct AST_expr_stmt : public Stmt{
    Expr* expression = nullptr;
    AST_expr_stmt(Expr* expr) : expression(expr){}

    void print() override {
        std::cout << "[EXPRESSION_STATEMENT] ";
        if (expression) {
            std::cout << expression->printString();
        } else {
            std::cout << "null";
        }
        std::cout << std::endl;
    }

    void execute() override;
};

struct AST_func_call : public Expr{
    std::string function_name;
    std::vector<Expr*> function_args;

    bool _builtIn;

    std::string printString() override {
        std::string out = "Call(" + function_name + "(";
        for (size_t i = 0; i < function_args.size(); ++i) {
            if (function_args[i]) {
                out += function_args[i]->printString();
            } else {
                out += "null";
            }
            if (i != function_args.size() - 1) {
                out += ", ";
            }
        }
        out += "))";
        return out;
    }

    Value getValue() override;
};

struct AST_identifier : public Expr{
    std::string identifier_name;

    std::string printString() override {
        return identifier_name;
    }
    Value getValue() override;
};

struct AST_literal : public Expr{
    Value literal_value;

    std::string printString() override {
        return "Literal(" + literal_value.toString() + ")";
    }
    Value getValue() override;
};
struct AST_binary_expression : public Expr{
    std::string op;
    Expr* left = nullptr;
    Expr* right = nullptr;

    std::string printString() override {
        std::string lstr = (left ? left->printString() : "null");
        std::string rstr = (right ? right->printString() : "null");
        return "(" + lstr + " " + op + " " + rstr + ")";
    }
    Value getValue() override;
};

struct AST_unary_expression : public Expr{
    std::string op;
    Expr* expr = nullptr;

    std::string printString() override {
        return "(" + op + expr->printString() + ")";
    }
    Value getValue() override;
};

