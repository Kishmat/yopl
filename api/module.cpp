#include "module.h"
#include "../env/environment.h"


YOPL_Value convertToPublic(const Value &v)
{
    YOPL_Value out{YOPL_TYPE_NULL};
    switch (v.type)
    {
    case Value::Type::NUMBER:
    {
        out.type = YOPL_TYPE_NUMBER;
        const Number& num = v.as_number();
        YOPL_Number nums;
        if(num.type == Number::Type::INTEGER)
        {
            nums.type = YOPL_Number::INTEGER;
            nums.i = num.as_int();
            out.number = nums;
        }else{
            nums.type = YOPL_Number::DECIMAL;
            nums.i = num.as_double();
            out.number = nums;
        }
        break;
    }
    case Value::Type::STRING:{
        out.type = YOPL_TYPE_STRING;
        out.string = v.as_string().c_str();
        break;
    }
    case Value::Type::BOOL:{
        out.type = YOPL_TYPE_BOOL;
        out.boolean = v.as_bool();
        break;
    }
    case Value::Type::REFERENCE:{
        out.type = YOPL_TYPE_REFERENCE;
        YOPL_Value* ref = (YOPL_Value*)v.as_reference();
        out.reference = ref;
        break;
    }
    case Value::Type::ARRAY:{
        out.type = YOPL_TYPE_ARRAY;
        Array* old = v.as_array();
        YOPL_Array* arr = new YOPL_Array();
        arr->size = static_cast<int>(old->size);
        if(arr->size > 0){
            for (int i = 0; i < arr->size; ++i) {
                YOPL_Value* v = new YOPL_Value(convertToPublic(*old->elements[i]));
                arr->elements.push_back(v);
            }
        }
        out.array = arr;
        break;
    }
    default:
        break;
    }
    return out;
}

Value convertFromPublic(const YOPL_Value &v)
{
    switch (v.type)
    {
    case YOPL_TYPE_NUMBER:
    {
        const YOPL_Number& num = v.number;
        if(num.type == YOPL_Number::INTEGER)
            return Value(num.i);
        else
            return Value(num.d);
    }
    case YOPL_TYPE_STRING:
    {
        std::string str = v.string;
        return Value(str);
    }
    case YOPL_TYPE_BOOL:
        return Value(v.boolean);
    case YOPL_TYPE_REFERENCE:
        return Value((Value*)v.reference);
    case YOPL_TYPE_ARRAY:{
        YOPL_Array* old = v.array;
        std::vector<Value*> vs;
        for(int i = 0;i < old->size; i++){
            Value* va = new Value(convertFromPublic(v));
            vs.push_back(va);
        }
        Array* arr = new Array(vs);
        return arr;
    }
    case YOPL_TYPE_ERROR:
    {
        std::string str = v.string;
        Value v;
        v.type = Value::Type::ERR;
        v.value = str;
        return v;
    }
    default:
        return Value();
    }
}