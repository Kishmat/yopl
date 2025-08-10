#pragma once
#include<string>
struct Token{
    enum class Type{
        // reserved
        KEYWORD,IDENTIFIER,FUNCTION_CALL,REFERENCE,

        // comparisons
        GT,LT,GTE,LTE,EQUALITY,NOT_EQUAL,AND,OR,

        // operations
        MOD,PLUS,MINUS,MULTIPLY,DIVIDE,INCREMENT,DECREMENT,
        
        //symbols 
        LPAREN,RPAREN,COMMA,CURLYL,CURLYR,EQUALS,SEMI,

        // datas
        NUMBER,STRING,BOOLEAN,

        // EOF
        END
    };
    Type type;
    std::string value;
};
std::string get_token_name(Token::Type type);