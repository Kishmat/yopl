#include "../api/module.h"
#include <string.h>
YOPL_DefineFunction(strlen){
    if(argc != 1)
        return YOPL_Error("Function strlen expects 1 argument");
    if(argv[0].type != YOPL_TYPE_STRING)
        return YOPL_Error("Function strlen expects arguemnt to be of type string");
    YOPL_Value length;
    length.setValue((int)strlen(argv[0].string));
    return length;
}

void registerModule(YOPL_RegisterFn regFn){
    regFn("strlen",strlen);
}