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

#ifndef __h3o_Action__
#define __h3o_Action__

#include <map>

#include "HClass.hpp"
#include "HContext.hpp"
#include "../vm/RuntimeContext.hpp"
#include "../vm/ActionData.hpp"
#include "../vm/glue.hpp"

namespace hydro
{

class HAction : public HObject, public RuntimeContext
{
    friend class HvmEnv;
    friend class VM;
    
private:
    const ActionData *_vaction;
    hvalue _thisObject;
    action_glue *_glue;
    
    HAction(HvmEnv *env, HClass *actionClass, const ActionData *vaction, HClass *ownerClass, HObject *instance, action_glue *glue);
    HAction(HvmEnv *env, HClass *actionClass, const ActionData *vaction, action_glue *glue);
    
    hvalue h3o_trigger(HvmContext *cxt, VM *vm, hvalue thisObject, hobject params, hcontext context);
    
public:
	virtual ~HAction();
    hvalue trigger(HvmContext *threadContext, VM *vm, std::map<std::string, hvalue> &params, hvalue thisObject = nullptr);
    hvalue trigger(HvmContext *threadContext, VM *vm, VMContext *eventContext, hvalue thisObject = nullptr);
    virtual uint16_t modifier() const override { return _vaction->modifier; }
    virtual std::string assemblyName() const override { return _vaction->name; }
    virtual runtime_context_type type() const override { return hvm_runtime_action; }
};

typedef object_ptr<HAction> haction;

} // namespace hydro

#endif /* __h3o_Action__ */
