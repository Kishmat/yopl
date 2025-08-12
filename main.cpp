#include<iostream>
#include "env/environment.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "visitor/visitor.h"

bool hasYosExtension(const std::string& filename) {
    return filename.size() >= 5 && filename.substr(filename.size() - 5) == ".yopl";
}
int main(int argc,char* argv[]){
    std::string filename;

    if (argc == 2) {
        filename = argv[1];
        if (!hasYosExtension(filename)) {
            std::cerr << "Error: File must have .yopl extension.\n";
            return 1;
        }
    }else {
        std::cerr << "Usage: yopl.exe [filename.yopl]\n";
        return 1;
    }
    Env::loadSource(filename);
    Lexer lexer;
    lexer.lex();
    // lexer.printTokens();
    // exit(-1);
    Parser parser;
    parser.parse();
    // parser.printNodes();
    // exit(-1);
    Visitor visitor;
    visitor.visit();
    return 0;
}