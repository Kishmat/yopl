#pragma once
#include<string>
struct Token{
    enum class Type{
        // reserved
        KEYWORD,IDENTIFIER,FUNCTION_CALL,

        // comparisons
        GT,LT,GTE,LTE,EQUALITY,NOT_EQUAL,

        // operations
        MOD,EQUALS,SEMI,PLUS,MINUS,MULTIPLY,DIVIDE,LPAREN,RPAREN,COMMA,
        CURLYL,CURLYR,

        // datas
        NUMBER,STRING,BOOLEAN,

        // EOF
        END
    };
    Type type;
    std::string value;
};
std::string get_token_name(Token::Type type);