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

#ifndef __h3o_HSetter__
#define __h3o_HSetter__

#include "../vm/RuntimeContext.hpp"
#include "../vm/VM_Setter.hpp"
#include "../vm/glue.hpp"

namespace hydro
{

class HSetter final : public HObject, public RuntimeContext
{
    friend class HvmEnv;
    
private:
	const VM_Setter *_vsetter;
	class HProperty *_ownerProperty;
	function_glue *_glue;
    
    HSetter(HvmEnv *env, HClass *setterClass, const VM_Setter *vsetter, HProperty *ownerProperty, function_glue *glue = nullptr);

public:
	virtual ~HSetter();
    void set(HvmContext *threadContext, VM *vm, HObject *instance, const hvalue value);
	virtual HProperty *ownerProperty() const { return _ownerProperty; }
	virtual uint16_t modifier() const override { return _vsetter->modifier; }
	virtual std::string assemblyName() const override { return ""; }
    virtual runtime_context_type type() const override { return hvm_runtime_setter; }
};

typedef object_ptr<HSetter> hsetter;

} // namespace hydro

#endif /* __h3o_Setter__ */
