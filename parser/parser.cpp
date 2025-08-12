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
            else if(current_token.value == "for")
                return parse_for_statement();
            else if(current_token.value == "while")
                return parse_while_statement();
            else if(current_token.value == "break" || current_token.value == "continue")
                return parse_control_flow_statement(current_token.value);
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
    if(current_token.type == Token::Type::REFERENCE)
    {
        node->isReference = true;
        parser_consume(Token::Type::REFERENCE);
        parser_consume(Token::Type::EQUALS);
        auto* id = new AST_identifier();
        id->identifier_name = current_token.value;
        parser_consume(Token::Type::IDENTIFIER);
        node->variable_value = id;
        parser_consume(Token::Type::SEMI);
        return node;
    }
    parser_consume(Token::Type::IDENTIFIER);
    if(current_token.type == Token::Type::SEMI)
    {
        parser_consume(Token::Type::SEMI);
        auto* exp = new AST_literal();
        exp->literal_value = Value();
        node->variable_value = exp;
        return node;
    }
    parser_consume(Token::Type::EQUALS);
    node->variable_value = parse_expression();
    parser_consume(Token::Type::SEMI);
    return node;
}
Stmt* Parser::parse_var_assign(){
    Token::Type t = Env::m_tokens[i+1].type;
    if(t == Token::Type::DOT)
    {
        auto* node = new AST_property_call();
        node->object = current_token.value;
        parser_consume(Token::Type::IDENTIFIER);
        parser_consume(Token::Type::DOT);
        if(current_token.type == Token::Type::IDENTIFIER)
        {
            node->isFunction = false;
            node->propertyName = current_token.value;
            parser_consume(Token::Type::IDENTIFIER);
        }else if(current_token.type == Token::Type::FUNCTION_CALL)
        {
            node->isFunction = true;
            node->propertyName = current_token.value;
            parser_consume(Token::Type::FUNCTION_CALL);
            parser_consume(Token::Type::LPAREN);
            while(current_token.type != Token::Type::RPAREN)
            {
                if(current_token.type == Token::Type::COMMA)
                parser_consume(Token::Type::COMMA);
                else
                node->args.push_back(parse_expression());
            }
            parser_consume(Token::Type::RPAREN);
        }
        parser_consume(Token::Type::SEMI);
        
        auto* parent = new AST_expr_stmt(node);
        return parent;
    }else{
        auto* node = new AST_var_assign();
        node->variable_name = current_token.value;
        parser_consume(Token::Type::IDENTIFIER);
        if(current_token.type == Token::Type::EQUALS)
        {
            parser_consume(Token::Type::EQUALS);
        }else if(current_token.type == Token::Type::BRACEL)
        {
            parser_consume(Token::Type::BRACEL);
            node->index = parse_expression();
            parser_consume(Token::Type::BRACER);
            parser_consume(Token::Type::EQUALS);
        }
        node->variable_value = parse_expression();
        parser_consume(Token::Type::SEMI);
        return node;
    }
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
            if(current_token.type == Token::Type::REFERENCE)
            {
                node->isReferenced.push_back(true);
                parser_consume(Token::Type::REFERENCE);
            }else{
                node->isReferenced.push_back(false);
                parser_consume(Token::Type::IDENTIFIER);
            }
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
Stmt* Parser::parse_for_statement(){
    auto* node = new AST_for_stmt();
    parser_consume(Token::Type::KEYWORD);
    parser_consume(Token::Type::LPAREN);
    node->idName = current_token.value;
    parser_consume(Token::Type::IDENTIFIER);
    parser_consume(Token::Type::EQUALS);
    node->start = parse_expression();
    parser_consume(Token::Type::KEYWORD);
    node->end = parse_expression();
    parser_consume(Token::Type::RPAREN);
    parser_consume(Token::Type::CURLYL);
    while(current_token.type != Token::Type::CURLYR)
    {
        node->body.push_back(parse_statement());
    }
    parser_consume(Token::Type::CURLYR);
    return node;
}
Stmt* Parser::parse_control_flow_statement(std::string& value){
    if(value == "break"){
        auto* node = new AST_break_statement;
        parser_ignore();
        parser_consume(Token::Type::SEMI);
        return node;
    }else{
        auto* node = new AST_continue_statement;
        parser_ignore();
        parser_consume(Token::Type::SEMI);
        return node;
    }
}
Stmt* Parser::parse_while_statement(){
    auto* node = new AST_while_stmt();
    parser_consume(Token::Type::KEYWORD);
    parser_consume(Token::Type::LPAREN);
    node->condition = parse_condition();
    parser_consume(Token::Type::RPAREN);
    parser_consume(Token::Type::CURLYL);
    while(current_token.type != Token::Type::CURLYR)
    {
        node->body.push_back(parse_statement());
    }
    parser_consume(Token::Type::CURLYR);
    return node;
}
Stmt* Parser::parse_if_statement(){
    auto* node = new AST_if_statement();
    parser_consume(Token::Type::KEYWORD);
    parser_consume(Token::Type::LPAREN);
    node->condition = parse_condition();
    parser_consume(Token::Type::RPAREN);
    parser_consume(Token::Type::CURLYL);
    while(current_token.type != Token::Type::CURLYR){
        node->if_body.push_back(parse_statement());
    }
    parser_consume(Token::Type::CURLYR);
    while(current_token.value == "elseif")
    {
        auto* elseif_node = new AST_if_statement();
        parser_consume(Token::Type::KEYWORD);
        parser_consume(Token::Type::LPAREN);
        elseif_node->condition = parse_condition();
        parser_consume(Token::Type::RPAREN);
        parser_consume(Token::Type::CURLYL);
        while (current_token.type != Token::Type::CURLYR) {
            elseif_node->if_body.push_back(parse_statement());
        }
        parser_consume(Token::Type::CURLYR);
        node->elseif_body.push_back(elseif_node);
    }
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
Expr* Parser::parse_condition(){
    Expr* left = parse_comparision();
    while(current_token.type == Token::Type::AND || 
        current_token.type == Token::Type::OR)
        {
            std::string op = current_token.value;
            parser_ignore();
            Expr* right = parse_comparision();

            auto* temp = new AST_binary_expression();
            temp->op = op;
            temp->left = left;
            temp->right = right;
            left = temp;
        }
    return left;
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
    if (current_token.type == Token::Type::NUMBER || current_token.type == Token::Type::STRING || current_token.type == Token::Type::BOOLEAN || current_token.type == Token::Type::NILL) {
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
        }else if(current_token.type == Token::Type::BOOLEAN){
            bool state = false;
            if(current_token.value == "true")
                state = true;
            node->literal_value = Value(state);
        }else{
            node->literal_value = Value();
        }
        parser_ignore();
        return node;
    } else if (current_token.type == Token::Type::IDENTIFIER) {
        Token::Type t = Env::m_tokens[i+1].type;
        if(t == Token::Type::DOT)
        {
            auto* node = new AST_property_call();
            node->object = current_token.value;
            parser_consume(Token::Type::IDENTIFIER);
            parser_consume(Token::Type::DOT);
            if(current_token.type == Token::Type::IDENTIFIER)
            {
                node->isFunction = false;
                node->propertyName = current_token.value;
                parser_consume(Token::Type::IDENTIFIER);
            }else if(current_token.type == Token::Type::FUNCTION_CALL)
            {
                node->isFunction = true;
                node->propertyName = current_token.value;
                parser_consume(Token::Type::FUNCTION_CALL);
                parser_consume(Token::Type::LPAREN);
                while(current_token.type != Token::Type::RPAREN)
                {
                    if(current_token.type == Token::Type::COMMA)
                    parser_consume(Token::Type::COMMA);
                    else
                    node->args.push_back(parse_expression());
                }
                parser_consume(Token::Type::RPAREN);
            }
            return node;
        }else if(t == Token::Type::BRACEL){
            auto* node = new AST_array_access();
            node->array_name = current_token.value;
            parser_consume(Token::Type::IDENTIFIER);
            parser_consume(Token::Type::BRACEL);
            node->index = parse_expression();
            parser_consume(Token::Type::BRACER);
            return node;
        }else{
            auto* node = new AST_identifier();
            node->identifier_name = current_token.value;
            parser_ignore();
            return node;
        }
    }else if (current_token.type == Token::Type::LPAREN) {
        parser_consume(Token::Type::LPAREN);
        Expr* inner = parse_expression();
        parser_consume(Token::Type::RPAREN);
        return inner;
    }else if (current_token.type == Token::Type::BRACEL) {
        auto* node = new AST_array();
        parser_consume(Token::Type::BRACEL);
        while(current_token.type != Token::Type::BRACER)
        {
            if(current_token.type == Token::Type::COMMA)
                parser_consume(Token::Type::COMMA);
            else
                node->elements.push_back(parse_expression());
        }
        parser_consume(Token::Type::BRACER);
        return node;
    }else if(current_token.type == Token::Type::FUNCTION_CALL){
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
    if(current_token.type == Token::Type::INCREMENT ||
        current_token.type == Token::Type::DECREMENT){
        auto* temp = new AST_binary_expression();

        auto* left = new AST_identifier();
        left->identifier_name = Env::m_tokens[i-1].value;
        temp->op = current_token.value;

        temp->left = left;
        parser_ignore();
        return temp;
    }
    Expr* left = parse_term();
    while (current_token.type == Token::Type::PLUS || 
        current_token.type == Token::Type::MINUS) {
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