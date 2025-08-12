#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<variant>

struct Array;

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
        REFERENCE,
        ARRAY
    };
    Type type;

    std::variant<std::monostate,Number,std::string,bool,Value*,Array*> value;

    Value() : type(Type::NILL),value(std::monostate{}) {}
    Value(std::string& str) : type(Type::STRING),value(str){}
    Value(const char* str) : type(Type::STRING),value(std::string(str)) {}
    Value(bool b) : type(Type::BOOL), value(b) {}
    Value(int i) : type(Type::NUMBER), value(Number(i)) {}
    Value(double d) : type(Type::NUMBER), value(Number(d)) {}
    Value(Value* ref) : type(Type::REFERENCE), value(ref) {}
    Value(Array* array) : type(Type::ARRAY), value(array) {}

    const std::string& as_string() const { return std::get<std::string>(value); }
    bool as_bool() const { return std::get<bool>(value); }
    const Number& as_number() const { return std::get<Number>(value); }
    Array* as_array() const { return std::get<Array*>(value); }

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
    std::string toString();
};
struct Array{
    std::vector<Value*> elements;
    size_t size;
    Array(std::vector<Value*>& arr) : elements(arr){
        size = elements.size();
    }
    void push(Value* v){
        elements.push_back(v);
        size = elements.size();
    }
    void pop(){
        elements.pop_back();
        size = elements.size();
    }
    bool empty(){
        return elements.empty();
    }
    Value* at(size_t index){
        if(index < size)
        {
            return elements[index];
        }else{
            std::cout << "Access of non-existing member of array" << std::endl;
            exit(-1);
        }
    }
    std::string toString() const{
        std::string out = "(Array) => {\n";
        for (const auto& value : elements) {
            out += value->toString() + ", ";
        }
        out += "\n}";
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
