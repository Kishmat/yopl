#pragma once
#include<string>
#include<iostream>
#include<variant>

struct Number{
    enum Type{
        INTEGER,
        DECIMAL
    };
    Type type;
    std::variant<int,double> value;

    Number(int v) : type(Type::INTEGER), value(v) {}
    Number(double v) : type(Type::DECIMAL), value(v) {}

    int as_int() const { return std::get<int>(value); }
    double as_double() const { return std::get<double>(value); }

    double getValue() const {
        if(type == Type::INTEGER)
            return as_int();
        else
            return as_double();
    }
};

struct Value
{
    enum Type{
        NUMBER,
        STRING,
        BOOL,
        NILL,
        REFERENCE
    };
    Type type;
    std::variant<std::monostate,Number,std::string,bool,Value*> value;

    Value() : type(Type::NILL),value(std::monostate{}) {}
    Value(std::string& str) : type(Type::STRING),value(str){}
    Value(const char* str) : type(Type::STRING),value(std::string(str)) {}
    Value(bool b) : type(Type::BOOL), value(b) {}
    Value(int i) : type(Type::NUMBER), value(Number(i)) {}
    Value(double d) : type(Type::NUMBER), value(Number(d)) {}
    Value(Value* ref) : type(Type::REFERENCE), value(ref) {}

    const std::string& as_string() const { return std::get<std::string>(value); }
    bool as_bool() const { return std::get<bool>(value); }
    const Number& as_number() const { return std::get<Number>(value); }
    Value* as_reference() const {
        if (type == Type::REFERENCE) {
            return std::get<Value*>(value);
        }
        return nullptr;  // Not a reference
    }
    bool isReference() const{
        if(type == Type::REFERENCE)
            return true;
        return false;
    }
    bool as_condition() const;
    std::string toString(){
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
            case Type::NILL:{
                out = " (NULL) ";
                break;
            }
            case Type::REFERENCE:
            {
                out = " (REFERENCE) -> " + (as_reference() ? as_reference()->toString() : "null");
                break;
            }
        }
        return out;
    }
};
bool isFloat(std::string& value);

Value operator+(const Value& lhs, const Value& rhs);
Value operator-(const Value& lhs, const Value& rhs);
Value operator*(const Value& lhs, const Value& rhs);
Value operator/(const Value& lhs, const Value& rhs);
Value operator%(const Value& lhs, const Value& rhs);
bool operator==(const Value& lhs, const Value& rhs);
bool operator!=(const Value& lhs, const Value& rhs);
bool operator<(const Value& lhs, const Value& rhs);
bool operator>(const Value& lhs, const Value& rhs);
bool operator<=(const Value& lhs, const Value& rhs);
bool operator>=(const Value& lhs, const Value& rhs);

Value function_input();
