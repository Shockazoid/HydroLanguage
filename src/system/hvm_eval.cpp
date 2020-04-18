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

#include "hvm_eval.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

hvalue hvm_eval(HvmEnv *env, HvmContext *cxt, hvalue hydroCode, std::string scope)
{
    hvalue result;
    MemorySpace *scopeToUse = nullptr; // default is "own" scope
    
    if(scope == "global")
        scopeToUse = env->globals();
    else if(scope == "local" && cxt->currentFrame->parentFrame)
        scopeToUse = cxt->currentFrame->parentFrame->locals;
    
    if(env->eval()->code(env->cast()->string(hydroCode), result, scopeToUse))
        return result;
    
    return undefined;
}

} // namespace hydro
