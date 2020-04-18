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

#ifndef __h3o_Function__
#define __h3o_Function__

#include "../memory/object_ptr.hpp"
#include "../vm/RuntimeContext.hpp"
#include "../vm/VM_Func.hpp"
#include "../vm/glue.hpp"
#include "../vm/HvmCallable.hpp"

namespace hydro
{

typedef object_ptr<class HFunction> hfunction;

class HFunction : public HObject, public RuntimeContext, public HvmCallable
{
	friend class HvmEnv;
    friend class VM;
    friend struct SetService;

private:
	const VM_Func *_vfunc;
	function_glue *_glue;
	hvalue _thisObject;

    HFunction(HvmEnv *env, hclass functionClass, const VM_Func *vfunc, function_glue *glue, std::string ownerPackage = "", hvalue thisObject = nullptr);
    
    hvalue h3o_call(HvmEnv *env, VM *vm, HvmContext *cxt, hvalue thisObject, hobject args);
    
public:
	virtual ~HFunction();
    hvalue call(hvalue thisObject = nullptr, hobject args = nullptr);    
    virtual hvalue call(HvmContext *threadContext, VM *vm, std::list<hvalue> &args, hvalue thisObject = nullptr) override;
    virtual hvalue call(HvmContext *threadContext, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject = nullptr) override;
	const VM_Func *vfunc() const { return _vfunc; }
    uint16_t arity() { return _vfunc->nargs; }
	virtual uint16_t modifier() const override { return _vfunc->modifier; }
	virtual std::string assemblyName() const override { return _vfunc->name; }
    virtual runtime_context_type type() const override { return hvm_runtime_function; }
};

} // namespace hydro

#endif /* __h3o_Function__ */
