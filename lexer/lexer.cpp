#include "lexer.h"
#include <iostream>


void Lexer::printTokens(){
    for (auto& token : Env::m_tokens)
    {
        std::string type = "";
        switch (token.type)
        {
            case Token::Type::GT:{type = "GT";break;}
            case Token::Type::AND:{type = "AND";break;}
            case Token::Type::OR:{type = "OR";break;}
            case Token::Type::LT:{type = "LT";break;}
            case Token::Type::GTE:{type = "GTE";break;}
            case Token::Type::LTE:{type = "LTE";break;}
            case Token::Type::EQUALITY:{type = "EQUALITY";break;}
            case Token::Type::NOT_EQUAL:{type = "NOT_EQUAL";break;}
            case Token::Type::NUMBER:{type = "NUMBER";break;}
            case Token::Type::STRING:{type = "STRING";break;}
            case Token::Type::NILL:{type = "NULL";break;}
            case Token::Type::BOOLEAN:{type = "BOOLEAN";break;}
            case Token::Type::KEYWORD:{type = "KEYWORD";break;}
            case Token::Type::IDENTIFIER:{type = "IDENTIFIER";break;}
            case Token::Type::REFERENCE:{type = "REFERENCE";break;}
            case Token::Type::FUNCTION_CALL:{type = "FUNCTION_CALL";break;}
            case Token::Type::END:{type = "END";break;}
            case Token::Type::MOD:{type = "MOD";break;}
            case Token::Type::EQUALS:{type = "EQUALS";break;}
            case Token::Type::SEMI:{type = "SEMI";break;}
            case Token::Type::BRACEL:{type = "BRACEL";break;}
            case Token::Type::BRACER:{type = "BRACER";break;}
            case Token::Type::PLUS:{type = "PLUS";break;}
            case Token::Type::MINUS:{type = "MINUS";break;}
            case Token::Type::MULTIPLY:{type = "MULTIPLY";break;}
            case Token::Type::INCREMENT:{type = "INCREMENT";break;}
            case Token::Type::DECREMENT:{type = "DECREMENT";break;}
            case Token::Type::DIVIDE:{type = "DIVIDE";break;}
            case Token::Type::LPAREN:{type = "LPAREN";break;}
            case Token::Type::RPAREN:{type = "RPAREN";break;}
            case Token::Type::COMMA:{type = "COMMA";break;}
            case Token::Type::DOT:{type = "DOT";break;}
            case Token::Type::CURLYL:{type = "CURLYL";break;}
            case Token::Type::CURLYR:{type = "CURLYR";break;}
        }
        std::cout << "[" << type << "] => " << token.value << std::endl;
    }
}

void Lexer::lex(){
    while(ch != '\0')
    {
        if(ch == ' ' || ch == '\n')
            skip_whitespace();
        switch(ch){
            case '\"': {collect_string(true);break;}
            case '\'': {collect_string(false);break;}
            case '%': {Env::m_tokens.push_back({Token::Type::MOD, "%"});break;}
            case '!': {
                if(next_ch() == '=')
                {
                    advance();
                    Env::m_tokens.push_back({Token::Type::NOT_EQUAL, "!="});
                }
                break;
            }
            case '>': {
                if(next_ch() == '=')
                {
                    advance();
                    Env::m_tokens.push_back({Token::Type::GTE, ">="});
                }
                else
                    Env::m_tokens.push_back({Token::Type::GT, ">"});
                break;
            }
            case '<': {
                if(next_ch() == '=')
                {
                    advance();
                    Env::m_tokens.push_back({Token::Type::LTE, "<="});
                }
                else
                    Env::m_tokens.push_back({Token::Type::LT, "<"});
                break;
            }
            case '=': {
                if(next_ch() == '=')
                {
                    advance();
                    Env::m_tokens.push_back({Token::Type::EQUALITY, "=="});
                }
                else
                    Env::m_tokens.push_back({Token::Type::EQUALS, "="});
                break;
            }
            case ';': {Env::m_tokens.push_back({Token::Type::SEMI, ";"});break;}
            case '+': {
                if(next_ch() == '+'){
                    advance();
                    Env::m_tokens.push_back({Token::Type::INCREMENT, "++"});
                }else{
                    Env::m_tokens.push_back({Token::Type::PLUS, "+"});
                }
                break;
            }
            case '-': {
                if(next_ch() == '-'){
                    advance();
                    Env::m_tokens.push_back({Token::Type::DECREMENT, "--"});
                }else{
                    Env::m_tokens.push_back({Token::Type::MINUS, "-"});
                }
                break;
            }
            case '*': {Env::m_tokens.push_back({Token::Type::MULTIPLY, "*"});break;}
            case '/':{
                if(next_ch() == '/')
                    skip_comment();
                else
                    Env::m_tokens.push_back({Token::Type::DIVIDE, "/"});
                break;
            }

            case '(': {Env::m_tokens.push_back({Token::Type::LPAREN, "("});break;}
            case ')': {Env::m_tokens.push_back({Token::Type::RPAREN, ")"});break;}
            case ',': {Env::m_tokens.push_back({Token::Type::COMMA, ","});break;}
            case '.': {Env::m_tokens.push_back({Token::Type::DOT, "."});break;}
            case '{': {Env::m_tokens.push_back({Token::Type::CURLYL, "{"});break;}
            case '}': {Env::m_tokens.push_back({Token::Type::CURLYR, "}"});break;}
            case '[': {Env::m_tokens.push_back({Token::Type::BRACEL, "["});break;}
            case ']': {Env::m_tokens.push_back({Token::Type::BRACER, "]"});break;}
            case '|': {
                if(next_ch() == '|'){
                    advance();
                    Env::m_tokens.push_back({Token::Type::OR, "||"});
                }
                break;
            }
            case '&': {
                if(next_ch() == '&')
                {
                    advance();
                    Env::m_tokens.push_back({Token::Type::AND, "&&"});
                    break;
                }
                advance();
                std::string temp = "";
                if(isalnum(ch) || ch == '_'){
                    while(isalnum(ch) || ch == '_')
                    {
                        temp += ch;
                        advance();
                    }
                    i--;
                    Env::m_tokens.push_back({Token::Type::REFERENCE, temp});break;
                }
            }
        }
        if(isalnum(ch) || ch == '_'){
            if(isdigit(ch))
            {
                std::string number = collect_number();
                Env::m_tokens.push_back({Token::Type::NUMBER,number});
            }else{
                std::string temp = "";
                while(isalnum(ch) || ch == '_')
                {
                    temp += ch;
                    advance();
                }
                lex_identifier(temp);
            }
            i--;
        }
        advance();
    }
    Env::m_tokens.push_back({Token::Type::END,"EOF"});
}

char Lexer::next_ch(){
    if(i < Env::source_length)
    {
        return Env::m_source[i+1];
    }
    return '\0';
}
void Lexer::advance(){
    if(i < Env::source_length)
    {
        ch = Env::m_source[++i];
    }
}
void Lexer::skip_whitespace(){
    while(ch == ' ' || ch == '\n')
        advance();
}
void Lexer::skip_comment(){
    while (ch != '\n' && ch != '\0')
        advance();
}
void Lexer::collect_string(bool doubleQ){
    advance();
    std::string temp = "";
    while (true && ch != '\0')
    {
        if(doubleQ && ch == '\"')
            break;
        else if(!doubleQ && ch == '\'')
            break;
        temp += ch;
        advance();
    }
    Env::m_tokens.push_back({Token::Type::STRING, temp});
}
void Lexer::lex_identifier(std::string& value){
    if(Env::keywords.count(value))
    {
        Env::m_tokens.push_back({Token::Type::KEYWORD,value});
    }else if(value == "true" || value == "false")
    {
        Env::m_tokens.push_back({Token::Type::BOOLEAN,value});
    }else if(value == "null")
    {
        Env::m_tokens.push_back({Token::Type::NILL,value});
    }else{
        if(ch == '(')
        {
            Env::m_tokens.push_back({Token::Type::FUNCTION_CALL,value});
        }else{
            Env::m_tokens.push_back({Token::Type::IDENTIFIER,value});
        }
    }
}

std::string Lexer::collect_number(){
    std::string number;
    while(isdigit(ch) && ch != '\0')
    {
        number += ch;
        advance();
    }
    if(ch == '.' && i+1 < Env::source_length && isdigit(next_ch()) && ch != '\0')
    {
        number += ch;
        advance();
        while (isdigit(ch)) {
            number += ch;
            advance();
        }
    }
    return number;
}
std::string get_token_name(Token::Type type){
    switch (type)
    {
        case Token::Type::KEYWORD: return "keyword";
        case Token::Type::IDENTIFIER: return "identifier";
        case Token::Type::REFERENCE: return "reference";
        case Token::Type::FUNCTION_CALL: return "function";
        case Token::Type::GT: return ">";
        case Token::Type::AND: return "&&";
        case Token::Type::OR: return "||";
        case Token::Type::LT: return "<";
        case Token::Type::GTE: return ">=";
        case Token::Type::LTE: return "<=";
        case Token::Type::EQUALITY: return "==";
        case Token::Type::NOT_EQUAL: return "!=";

        case Token::Type::MOD: return "%";
        case Token::Type::EQUALS: return "=";
        case Token::Type::PLUS: return "+";
        case Token::Type::MINUS: return "-";
        case Token::Type::INCREMENT: return "++";
        case Token::Type::DECREMENT: return "--";
        case Token::Type::MULTIPLY: return "*";
        case Token::Type::DIVIDE: return "/";
        case Token::Type::END: return "EOF";

        case Token::Type::SEMI: return ";";
        case Token::Type::LPAREN: return "(";
        case Token::Type::BRACEL: return "[";
        case Token::Type::BRACER: return "]";
        case Token::Type::RPAREN: return ")";
        case Token::Type::COMMA: return ",";
        case Token::Type::DOT: return ".";
        case Token::Type::CURLYL: return "{";
        case Token::Type::CURLYR: return "}";
        case Token::Type::NUMBER: return "number";
        case Token::Type::STRING: return "string";
        case Token::Type::NILL: return "null";
        case Token::Type::BOOLEAN: return "boolean";
    }
    return "nothing";
}