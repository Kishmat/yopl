#pragma once
#include<stddef.h>
#include<vector>
#ifdef _WIN32
#define YOPL_EXPORT __declspec(dllexport)
#else
#define YOPL_EXPORT
#endif


#ifdef __cplusplus
extern "C" {
#endif
#define YOPL_DefineFunction(name) static YOPL_Value name(int argc, YOPL_Value* argv)
#define YOPL_Error(msg) __error(msg)
typedef enum{
    YOPL_TYPE_NUMBER,
    YOPL_TYPE_STRING,
    YOPL_TYPE_BOOL,
    YOPL_TYPE_NULL,
    YOPL_TYPE_REFERENCE,
    YOPL_TYPE_ARRAY,
    YOPL_TYPE_ERROR
}YOPL_ValueType;

struct YOPL_Array;

typedef struct{
    enum Type{
        INTEGER,
        DECIMAL
    };
    Type type;
    union{
        int i;
        double d;
    };
    
}YOPL_Number;

struct YOPL_Value{
    YOPL_ValueType type = YOPL_TYPE_NULL;
    union{
        YOPL_Number number;
        const char* string;
        bool boolean;
        YOPL_Value* reference;
        YOPL_Array* array;
    };
    void setValue(int i){
        type = YOPL_TYPE_NUMBER;
        YOPL_Number num = {YOPL_Number::INTEGER};
        num.i = i;
        number = num;
    }
    void setValue(double d){
        type = YOPL_TYPE_NUMBER;
        YOPL_Number num = {YOPL_Number::DECIMAL};
        num.d = d;
        number = num;
    }
    void setValue(const char* str){
        type = YOPL_TYPE_STRING;
        string = str;
    }
    void setValue(YOPL_Value* ref){
        type = YOPL_TYPE_REFERENCE;
        reference = ref;
    }
    void setValue(YOPL_Array* arr){
        type = YOPL_TYPE_ARRAY;
        array = arr;
    }
};

static inline YOPL_Value __error(const char* message){
    return (YOPL_Value){ YOPL_TYPE_ERROR, .string = message };
}

struct YOPL_Array{
    int size;
    std::vector<YOPL_Value*> elements;
};


// ---- function pointer types -----
typedef YOPL_Value (*YOPL_NativeFn)(int argc, YOPL_Value* argv);
typedef void (*YOPL_RegisterFn)(const char* name,YOPL_NativeFn fn);

// ----------- MODULE entry point -------
YOPL_EXPORT void registerModule(YOPL_RegisterFn regFn);

#ifdef __cplusplus
}
#endif