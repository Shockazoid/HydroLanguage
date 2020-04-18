//
//                 __  __            __           
//                / / / /__  __ ____/ /_____ ____ 
//               / /_/ // / / // __  // ___// __ \
//              / __  // /_/ // /_/ // /   / /_/ /
//             /_/ /_/ \__, / \__,_//_/    \____/ 
//                    /____/                      
//
//              The Hydro Programming Language
//
//        © 2020 Shockazoid, Inc. All Rights Reserved.
//

#include "HConstructor.hpp"

namespace hydro
{

HConstructor::HConstructor(HvmEnv *env, HClass *constructorClass, const ConstructorData *vconstruct, HClass *ownerClass, function_glue *glue) : RuntimeContext{ownerClass}, HObject{env, constructorClass}, _vconstruct{vconstruct}, _glue{glue} {}

HConstructor::~HConstructor() {}
hvalue HConstructor::call(HvmContext *threadContext, VM *vm, std::list<hvalue> &args, hvalue thisObject)
{
    assert(!is_null(thisObject));
    return _glue->call(threadContext, this, vm, args, thisObject);
}

hvalue HConstructor::call(HvmContext *threadContext, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject)
{
    assert(!is_null(thisObject));
    return _glue->call(threadContext, this, vm, args, thisObject);
}


} // namespace hydro
