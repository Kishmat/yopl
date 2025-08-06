#pragma once
#include "../env/environment.h"

class Parser{
private:
    size_t i;
    Token current_token;
public:
    Parser(){
        i = 0;
        current_token = Env::m_tokens[0];
    }
    void parse();
    void printNodes();
private:
    Stmt* parse_statement();
    Stmt* parse_var_def();
    Stmt* parse_var_assign();
    Stmt* parse_func_def();
    Stmt* parse_if_statement();
    Stmt* parse_return_statement();

    Expr* parse_func_call();
    Expr* parse_expression();
    Expr* parse_term();
    Expr* parse_factor();
    Expr* parse_comparision();

    void parser_consume(Token::Type type);
    void parser_ignore();

    void print_exception(Token::Type expected, Token::Type got);
};