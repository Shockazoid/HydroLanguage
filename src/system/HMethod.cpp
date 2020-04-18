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

#include "HMethod.hpp"
#include "VM.hpp"

namespace hydro
{

HMethod::HMethod(HvmEnv *env, HClass *methodClass, const VM_Method *vmethod, HClass *ownerClass, function_glue *glue) : HObject{env, methodClass}, RuntimeContext{ownerClass}, _vmethod{vmethod}, _glue{glue} {}

HMethod::~HMethod() {}

hvalue HMethod::call(HvmEnv *env, VM *vm, HvmContext *threadContext, std::list<hvalue> &args, hvalue thisObject)
{
    return _glue->call(threadContext, this, vm, args, thisObject);
}

} // namespace hydro
