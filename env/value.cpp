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


std::string Value::toString(){
        std::string out = "";
        switch (type)
        {
            case Type::NUMBER:{
                Number num = as_number();
                if(num.type == Number::Type::INTEGER)
                {
                    out = " (Int) " + std::to_string(num.as_int());
                }else{
                    out = " (Float) " + std::to_string(num.as_double());
                }
                break;
            }
            case Type::STRING:{
                out = " (String) " + as_string();
                break;
            }
            case Type::BOOL:{
                out = " (Boolean) ";
                bool state = as_bool();
                if(state)
                    out += "True";
                else
                    out += "False";
                break;
            }
            case Type::ARRAY:{
                const Array* arr = as_array();
                if(arr){
                    out = arr->toString();
                }
                break;
            }
            case Type::REFERENCE:
            {
                out = " (REFERENCE) -> " + (as_reference() ? as_reference()->toString() : "null");
                break;
            }
            case Type::NILL:{
                out = " (NULL) ";
                break;
            }
            default: break;
        }
        return out;
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
    const Value& lhs_value = lhs.isReference() ? *lhs.as_reference() : lhs;
    const Value& rhs_value = rhs.isReference() ? *rhs.as_reference() : rhs;


    if(lhs_value.type == Value::Type::NUMBER && rhs_value.type == Value::Type::NUMBER){
        const Number& lnum = lhs_value.as_number();
        const Number& rnum = rhs_value.as_number();

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
    else if(lhs_value.type == Value::Type::STRING && rhs_value.type == Value::Type::STRING){
        std::string concated = lhs_value.as_string() + rhs_value.as_string();
        return Value(concated);
    }
    else if(lhs_value.type == Value::Type::STRING && rhs_value.type == Value::Type::NUMBER)
    {
        const Number& r = rhs_value.as_number();
        std::string concated = lhs_value.as_string();
        if(r.type == Number::Type::DECIMAL){
            concated += std::to_string(r.as_double());
        }else{
            concated += std::to_string(r.as_int());
        }
        return concated;
    }else if(lhs_value.type == Value::Type::NUMBER && rhs_value.type == Value::Type::STRING){
        const Number& r = lhs_value.as_number();
        std::string concated;
        if(r.type == Number::Type::DECIMAL){
            concated = std::to_string(r.as_double());
        }else{
            concated = std::to_string(r.as_int());
        }
        concated += rhs_value.as_string();
        return concated;
    }else{
        std::cout << "Invalid operation + on unsupported types!" << std::endl; 
        exit(-1);
    }
}

Value operator-(const Value& lhs, const Value& rhs){
    const Value& lhs_value = lhs.isReference() ? *lhs.as_reference() : lhs;
    const Value& rhs_value = rhs.isReference() ? *rhs.as_reference() : rhs;
    if(lhs_value.type == Value::Type::NUMBER && rhs_value.type == Value::Type::NUMBER){
        const Number& lnum = lhs_value.as_number();
        const Number& rnum = rhs_value.as_number();

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
    const Value& lhs_value = lhs.isReference() ? *lhs.as_reference() : lhs;
    const Value& rhs_value = rhs.isReference() ? *rhs.as_reference() : rhs;
    if(lhs_value.type == Value::Type::NUMBER && rhs_value.type == Value::Type::NUMBER){
        const Number& lnum = lhs_value.as_number();
        const Number& rnum = rhs_value.as_number();

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
    const Value& lhs_value = lhs.isReference() ? *lhs.as_reference() : lhs;
    const Value& rhs_value = rhs.isReference() ? *rhs.as_reference() : rhs;
    if(lhs_value.type == Value::Type::NUMBER && rhs_value.type == Value::Type::NUMBER){
        const Number& lnum = lhs_value.as_number();
        const Number& rnum = rhs_value.as_number();

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
    const Value& lhs_value = lhs.isReference() ? *lhs.as_reference() : lhs;
    const Value& rhs_value = rhs.isReference() ? *rhs.as_reference() : rhs;
    if(lhs_value.type == Value::Type::NUMBER && rhs_value.type == Value::Type::NUMBER){
        const Number& lnum = lhs_value.as_number();
        const Number& rnum = rhs_value.as_number();

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
    const Value& lhs_value = lhs.isReference() ? *lhs.as_reference() : lhs;
    const Value& rhs_value = rhs.isReference() ? *rhs.as_reference() : rhs;
    if(lhs_value.type != rhs_value.type) return false;
    switch (lhs_value.type)
    {
        case Value::NUMBER: {
            const Number& lnum = lhs_value.as_number();
            const Number& rnum = rhs_value.as_number();
            if(lnum.type != rnum.type) return false;
            if(lnum.type == Number::Type::INTEGER)
            {
                return (lnum.as_int() == rnum.as_int());
            }else{
                return (lnum.as_double() == rnum.as_double());
            }
        }
        case Value::BOOL: return lhs_value.as_bool() == rhs_value.as_bool();
        case Value::STRING: return lhs_value.as_string() == rhs_value.as_string();
        case Value::NILL: return true; 
        default: break;
    }
    return false;
}

bool operator!=(const Value& lhs, const Value& rhs){
    return !(lhs == rhs);
}
bool operator<(const Value& lhs, const Value& rhs){
    const Value& lhs_value = lhs.isReference() ? *lhs.as_reference() : lhs;
    const Value& rhs_value = rhs.isReference() ? *rhs.as_reference() : rhs;
    if(lhs_value.type != rhs_value.type) throw std::runtime_error("Cannot compare different datatypes");
    switch (lhs_value.type)
    {
        case Value::NUMBER: {
            const Number& lnum = lhs_value.as_number();
            const Number& rnum = rhs_value.as_number();
            if(lnum.type != rnum.type) return false;
            if(lnum.type == Number::Type::INTEGER)
            {
                return (lnum.as_int() < rnum.as_int());
            }else{
                return (lnum.as_double() < rnum.as_double());
            }
        }
        case Value::STRING: return lhs_value.as_string() < rhs_value.as_string();
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