#pragma once
#include<string>
#include "../env/environment.h"
class Preprocessor
{
private:
    std::string m_source;
    std::string sourceCode;
public:
    Preprocessor(std::string& file){
        fs::path input_path = file;
        fs::path wd = fs::absolute(input_path).parent_path();
        if (wd.empty()) {
            wd = ".";
        }
        Env::working_directory = wd;
        fs::path sourceFile =  file;
        m_source = Env::getSourceCode(sourceFile);
    }
    void scan();
private:
    std::string trim(const std::string& s);
    void processSource(const std::string& source);
};