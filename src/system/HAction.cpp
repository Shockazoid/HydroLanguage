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

#include "HAction.hpp"

namespace hydro
{

HAction::HAction(HvmEnv *env, HClass *actionClass, const VM_Action *vaction, HClass *ownerClass, HObject *instance, action_glue *glue) : HObject{env, actionClass}, RuntimeContext{ownerClass}, _vaction{vaction}, _thisObject{instance}, _glue{glue} {}

HAction::HAction(HvmEnv *env, HClass *actionClass, const VM_Action *vaction, action_glue *glue) : HObject{env, actionClass}, RuntimeContext{}, _vaction{vaction}, _thisObject{nullptr}, _glue{glue} {}

HAction::~HAction() {}

hvalue HAction::h3o_trigger(HvmContext *cxt, VM *vm, hvalue thisObject, hobject params, hcontext context)
{
    std::map<std::string, hvalue> mParams;
    if(params)
        params->context()->copyToMap(mParams);
    // TODO trigger with custom context
    return _glue->trigger(cxt, this, vm, mParams, thisObject);
}

hvalue HAction::trigger(HvmContext *threadContext, VM *vm, std::map<std::string, hvalue> &params, hvalue thisObject)
{
    if(_thisObject)
        thisObject = _thisObject;
    
    return _glue->trigger(threadContext, this, vm, params, thisObject);
}

hvalue HAction::trigger(HvmContext *threadContext, VM *vm, VMContext *eventContext, hvalue thisObject)
{
    return false;
}

} // namespace hydro
