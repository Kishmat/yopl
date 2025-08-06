#include "parser.h"
#include <iostream>

void Parser::parse(){
    while(current_token.type != Token::Type::END){
        Env::m_nodes.push_back(parse_statement());
    }
}
void Parser::printNodes(){
    for(auto* node : Env::m_nodes)
    {
        node->print();
        std::cout << "\n";
    }
}

void Parser::parser_consume(Token::Type type){
    if(current_token.type == type)
    {
        if(i <= Env::m_tokens.size())
            current_token = Env::m_tokens[++i];
    }else{
        print_exception(type,current_token.type);
        exit(-1);
    }
}
void Parser::parser_ignore(){
    if(i <= Env::m_tokens.size())
        current_token = Env::m_tokens[++i];
}

Stmt* Parser::parse_statement(){
    switch (current_token.type)
    {
        case Token::Type::KEYWORD:{
            if(current_token.value == "let")
                return parse_var_def();
            else if(current_token.value == "if")
                return parse_if_statement();
            else if(current_token.value == "function")
                return parse_func_def();
            else if(current_token.value == "return")
                return parse_return_statement();
            break;
        }
        case Token::Type::FUNCTION_CALL:{
            Expr* expr = parse_expression();
            parser_consume(Token::Type::SEMI);
            return new AST_expr_stmt(expr);
            break;
        }
        case Token::Type::IDENTIFIER:{
            return parse_var_assign();
            break;
        }
        default:
            break;
    }
    return nullptr;
}
Stmt* Parser::parse_var_def(){
    auto* node = new AST_var_def();
    parser_consume(Token::Type::KEYWORD);
    node->variable_name = current_token.value;
    parser_consume(Token::Type::IDENTIFIER);
    parser_consume(Token::Type::EQUALS);

    node->variable_value = parse_expression();
    parser_consume(Token::Type::SEMI);
    return node;
}
Stmt* Parser::parse_var_assign(){
    auto* node = new AST_var_assign();
    node->variable_name = current_token.value;
    parser_consume(Token::Type::IDENTIFIER);
    parser_consume(Token::Type::EQUALS);
    node->variable_value = parse_expression();
    parser_consume(Token::Type::SEMI);
    return node;
}
Stmt* Parser::parse_func_def(){
    auto* node = new AST_func_def();
    parser_consume(Token::Type::KEYWORD);
    node->function_name = current_token.value;
    parser_consume(Token::Type::FUNCTION_CALL);
    parser_consume(Token::Type::LPAREN);

    while(current_token.type != Token::Type::RPAREN){
        if(current_token.type == Token::Type::COMMA)
            parser_ignore();
        else{
            node->function_params.push_back(current_token.value);
            parser_consume(Token::Type::IDENTIFIER);
        }
    }
    parser_consume(Token::Type::RPAREN);
    parser_consume(Token::Type::CURLYL);
    while(current_token.type != Token::Type::CURLYR)
    {
        node->function_body.push_back(parse_statement());
    }
    parser_consume(Token::Type::CURLYR);
    return node;
}
Expr* Parser::parse_func_call(){
    auto* node = new AST_func_call();
    node->function_name = current_token.value;
    bool builtIn = Env::isFunctionBuiltIn(current_token.value);
    node->_builtIn = builtIn;
    parser_consume(Token::Type::FUNCTION_CALL);
    parser_consume(Token::Type::LPAREN);
    while(current_token.type != Token::Type::RPAREN)
    {
        if(current_token.type == Token::Type::COMMA){
            parser_consume(Token::Type::COMMA);
        }else{
            node->function_args.push_back(parse_expression());
        }
    }
    parser_consume(Token::Type::RPAREN);
    return node;
}

Stmt* Parser::parse_return_statement(){
    auto* node = new AST_return_statement();
    parser_consume(Token::Type::KEYWORD);
    node->return_value = parse_comparision();
    parser_consume(Token::Type::SEMI);
    return node;
}

Stmt* Parser::parse_if_statement(){
    auto* node = new AST_if_statement();
    parser_consume(Token::Type::KEYWORD);
    parser_consume(Token::Type::LPAREN);
    node->condition = parse_comparision();
    parser_consume(Token::Type::RPAREN);
    parser_consume(Token::Type::CURLYL);
    while(current_token.type != Token::Type::CURLYR){
        node->if_body.push_back(parse_statement());
    }
    parser_consume(Token::Type::CURLYR);
    if(current_token.value == "else"){
        parser_consume(Token::Type::KEYWORD);
        parser_consume(Token::Type::CURLYL);
        while(current_token.type != Token::Type::CURLYR){
            node->else_body.push_back(parse_statement());
        }
        parser_consume(Token::Type::CURLYR);
    }
    return node;
}
Expr* Parser::parse_comparision(){
    if(current_token.type == Token::Type::SEMI)
    {
        auto* lit = new AST_literal();
        lit->literal_value = Value();
        return lit;
    }
    Expr* left = parse_expression();
    while (current_token.type == Token::Type::EQUALITY 
        || current_token.type == Token::Type::NOT_EQUAL
        || current_token.type == Token::Type::LT
        || current_token.type == Token::Type::LTE
        || current_token.type == Token::Type::GT
        || current_token.type == Token::Type::GTE){
            std::string op = current_token.value;
            parser_ignore();
            Expr* right = parse_expression();

            auto* temp = new AST_binary_expression();
            temp->op = op;
            temp->left = left;
            temp->right = right;
            left = temp;
        }
        return left;
}

Expr* Parser::parse_factor(){
    if(current_token.type == Token::Type::MINUS){
        parser_consume(Token::Type::MINUS);
        auto* node = new AST_unary_expression();
        node->op = "-";
        node->expr = parse_factor();
        return node;
    }
    if (current_token.type == Token::Type::NUMBER || current_token.type == Token::Type::STRING || current_token.type == Token::Type::BOOLEAN) {
        auto* node = new AST_literal();
        if(current_token.type == Token::Type::NUMBER)
        {
            if(isFloat(current_token.value))
            {
                node->literal_value = Value(std::stod(current_token.value));
            }else{
                node->literal_value = Value(std::stoi(current_token.value));
            }
        }else if(current_token.type == Token::Type::STRING){
            node->literal_value = Value(current_token.value);
        }else{
            bool state = false;
            if(current_token.value == "true")
                state = true;
            node->literal_value = Value(state);
        }
        parser_ignore();
        return node;
    } else if (current_token.type == Token::Type::IDENTIFIER) {
        auto* node = new AST_identifier();
        node->identifier_name = current_token.value;
        parser_ignore();
        return node;
    } else if (current_token.type == Token::Type::LPAREN) {
        parser_consume(Token::Type::LPAREN);
        Expr* inner = parse_expression();
        parser_consume(Token::Type::RPAREN);
        return inner;
    } else if(current_token.type == Token::Type::FUNCTION_CALL){
        auto* node = parse_func_call();
        return node;
    }else {
        std::cout << "Unexpected token found! : " << get_token_name(current_token.type) << std::endl;
        exit(-1);
    }
}

Expr* Parser::parse_term(){
    Expr* left = parse_factor();
    while (current_token.type == Token::Type::MULTIPLY || current_token.type == Token::Type::DIVIDE || current_token.type == Token::Type::MOD) {
        std::string op = current_token.value;
        parser_ignore();
        Expr* right = parse_factor();

        auto* temp = new AST_binary_expression();
        temp->op = op;
        temp->left = left;
        temp->right = right;
        left = temp;
    }
    return left;
}


Expr* Parser::parse_expression(){
    Expr* left = parse_term();
    while (current_token.type == Token::Type::PLUS || current_token.type == Token::Type::MINUS) {
        std::string op = current_token.value;
        parser_ignore();
        Expr* right = parse_term();

        auto* temp = new AST_binary_expression();
        temp->op = op;
        temp->left = left;
        temp->right = right;
        left = temp;
    }
    return left;
}
void Parser::print_exception(Token::Type expected, Token::Type got){
    std::cout << "Expected " << get_token_name(expected) << " but got " << get_token_name(got) << std::endl;
}