#include "preprocessor.h"
#include<sstream>
#include<fstream>
void Preprocessor::scan(){
    processSource(m_source);
    m_source = "";
    Env::m_source = sourceCode;
    Env::source_length = Env::m_source.length();
    sourceCode = "";
}
void Preprocessor::processSource(const std::string& src){
    std::istringstream stream(src);
    std::string line;

    while(std::getline(stream,line))
    {
        std::string stripped = trim(line);
        if(stripped.empty()) continue;
        if (stripped.rfind("include", 0) == 0) {
            size_t firstQuote = stripped.find('"');
            size_t lastQuote  = stripped.find_last_of('"');
            if (firstQuote != std::string::npos && lastQuote > firstQuote) {
                std::string filename = stripped.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                fs::path filepath = Env::working_directory / filename;
                auto result = Env::includedFiles.insert(filepath);
                if(!result.second)
                    continue;
                std::ifstream file(filepath);
                if (file) {
                    std::stringstream buffer;
                    buffer << file.rdbuf();
                    processSource(buffer.str());
                } else {
                    std::cerr << "Error: cannot open file " << filename << "\n";
                    exit(-1);
                }
            }else {
                std::cout << "Unknown preprocessor directive: " << stripped << "\n";
                exit(-1);
            }
            continue;
        }
        sourceCode += line+"\n";
    }
}
std::string Preprocessor::trim(const std::string& s){
    size_t start = s.find_first_not_of(" \t\r\n");
    if(start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start+1);
}