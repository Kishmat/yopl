#pragma once
#include "../env/environment.h"

class Lexer{
    private:
        size_t i;
        char ch;

    public:
        Lexer(){
            i = 0;
            ch = Env::m_source[0];
        }
        void lex();
        void printTokens();
    
    private:
        char next_ch();
        
        std::string collect_number();
        void advance();
        void skip_whitespace();
        void skip_comment();
        void collect_string(bool doubleQ);
        void lex_identifier(std::string& value);
};