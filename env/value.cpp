#include "value.h"
#include<regex>
bool isFloat(std::string& value)
{
    return value.find('.') != std::string::npos;
}
bool Value::as_condition() const{
    switch (type)
    {
        case Type::BOOL:{
            return as_bool();
            break;
        }
        case Type::STRING:{
            if(!as_string().empty())
                return true;
            break;
        }
        case Type::NUMBER:{
            Number num = as_number();
            if(num.type == Number::Type::INTEGER)
            {
                if(num.as_int() > 0)
                    return true;
            }else{
                if(num.as_double() > 0)
                    return true;
            }
            break;
        }
        default:{
            return false;
        }
    }
    return false;
}

Value function_input(){
    std::string input;
    std::getline(std::cin,input);

    if(input.empty()) return Value();

    std::regex intRegex(R"([+-]?\d+)");
    std::regex floatRegex(R"([+-]?\d*\.\d+([eE][+-]?\d+)?|[+-]?\d+\.\d*([eE][+-]?\d+)?)");

    if(std::regex_match(input,intRegex)){
        return Value(std::stoi(input));
    }else if(std::regex_match(input,floatRegex))
    {
        return Value(std::stod(input));
    }else{
        return Value(input);
    }
}
Value operator+(const Value& lhs, const Value& rhs){
    if(lhs.type == Value::Type::NUMBER && rhs.type == Value::Type::NUMBER){
        const Number& lnum = lhs.as_number();
        const Number& rnum = rhs.as_number();

        bool isDouble = false;

        if(lnum.type == Number::Type::DECIMAL || rnum.type == Number::Type::DECIMAL){
            isDouble = true;
        }
        double result = lnum.getValue() + rnum.getValue();
        if(isDouble){
            return Value(result);
        }
        return Value((int)(result));
    }
    else if(lhs.type == Value::Type::STRING && rhs.type == Value::Type::STRING){
        std::string concated = lhs.as_string() + rhs.as_string();
        return Value(concated);
    }
    else if(lhs.type == Value::Type::STRING && rhs.type == Value::Type::NUMBER)
    {
        const Number& r = rhs.as_number();
        std::string concated = lhs.as_string();
        if(r.type == Number::Type::DECIMAL){
            concated += std::to_string(r.as_double());
        }else{
            concated += std::to_string(r.as_int());
        }
        return concated;
    }else if(lhs.type == Value::Type::NUMBER && rhs.type == Value::Type::STRING){
        const Number& r = lhs.as_number();
        std::string concated;
        if(r.type == Number::Type::DECIMAL){
            concated = std::to_string(r.as_double());
        }else{
            concated = std::to_string(r.as_int());
        }
        concated += rhs.as_string();
        return concated;
    }else{
        std::cout << "Invalid operation + on unsupported types!" << std::endl; 
        exit(-1);
    }
}

Value operator-(const Value& lhs, const Value& rhs){
    if(lhs.type == Value::Type::NUMBER && rhs.type == Value::Type::NUMBER){
        const Number& lnum = lhs.as_number();
        const Number& rnum = rhs.as_number();

        bool isDouble = false;

        if(lnum.type == Number::Type::DECIMAL || rnum.type == Number::Type::DECIMAL){
            isDouble = true;
        }
        double result = lnum.getValue() - rnum.getValue();
        if(isDouble){
            return Value(result);
        }
        return Value((int)(result));
    }else{
        std::cout << "Invalid operation - on unsupported types!" << std::endl; 
        exit(-1);
    }
}

Value operator*(const Value& lhs, const Value& rhs){
    if(lhs.type == Value::Type::NUMBER && rhs.type == Value::Type::NUMBER){
        const Number& lnum = lhs.as_number();
        const Number& rnum = rhs.as_number();

        bool isDouble = false;

        if(lnum.type == Number::Type::DECIMAL || rnum.type == Number::Type::DECIMAL){
            isDouble = true;
        }
        double result = lnum.getValue() * rnum.getValue();
        if(isDouble){
            return Value(result);
        }
        return Value((int)(result));
    }else{
        std::cout << "Invalid operation * on unsupported types!" << std::endl; 
        exit(-1);
    }
}

Value operator/(const Value& lhs, const Value& rhs){
    if(lhs.type == Value::Type::NUMBER && rhs.type == Value::Type::NUMBER){
        const Number& lnum = lhs.as_number();
        const Number& rnum = rhs.as_number();

        bool isDouble = false;

        if(lnum.type == Number::Type::DECIMAL || rnum.type == Number::Type::DECIMAL){
            isDouble = true;
        }
        double result = lnum.getValue() / rnum.getValue();
        if(isDouble){
            return Value(result);
        }
        return Value((int)(result));
    }else{
        std::cout << "Invalid operation / on unsupported types!" << std::endl; 
        exit(-1);
    }
}

Value operator%(const Value& lhs, const Value& rhs){
    if(lhs.type == Value::Type::NUMBER && rhs.type == Value::Type::NUMBER){
        const Number& lnum = lhs.as_number();
        const Number& rnum = rhs.as_number();

        if(lnum.type == Number::Type::INTEGER && rnum.type == Number::Type::INTEGER){
            return Value(lnum.as_int() % rnum.as_int());
        }else{
            std::cout << "Invalid operation % on unsupported types!" << std::endl; 
            exit(-1);
        }
    }else{
        std::cout << "Invalid operation % on unsupported types!" << std::endl; 
        exit(-1);
    }
}

bool operator==(const Value& lhs, const Value& rhs){
    if(lhs.type != rhs.type) return false;
    switch (lhs.type)
    {
        case Value::NUMBER: {
            const Number& lnum = lhs.as_number();
            const Number& rnum = rhs.as_number();
            if(lnum.type != rnum.type) return false;
            if(lnum.type == Number::Type::INTEGER)
            {
                return (lnum.as_int() == rnum.as_int());
            }else{
                return (lnum.as_double() == rnum.as_double());
            }
        }
        case Value::BOOL: return lhs.as_bool() == rhs.as_bool();
        case Value::STRING: return lhs.as_string() == rhs.as_string();
        case Value::NILL: return true; 
    }
    return false;
}

bool operator!=(const Value& lhs, const Value& rhs){
    return !(lhs == rhs);
}
bool operator<(const Value& lhs, const Value& rhs){
    if(lhs.type != rhs.type) throw std::runtime_error("Cannot compare different datatypes");
    switch (lhs.type)
    {
        case Value::NUMBER: {
            const Number& lnum = lhs.as_number();
            const Number& rnum = rhs.as_number();
            if(lnum.type != rnum.type) return false;
            if(lnum.type == Number::Type::INTEGER)
            {
                return (lnum.as_int() < rnum.as_int());
            }else{
                return (lnum.as_double() < rnum.as_double());
            }
        }
        case Value::STRING: return lhs.as_string() < rhs.as_string();
        default:{
            std::cout << "Invalid operation < on invalid types\n";
            exit(-1);
        }
    }
    return false;
}
bool operator>(const Value& lhs, const Value& rhs){
    return rhs < lhs;
}

bool operator<=(const Value& lhs, const Value& rhs) {
    return !(rhs < lhs);
}

bool operator>=(const Value& lhs, const Value& rhs) {
    return !(lhs < rhs);
}