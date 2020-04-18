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

#ifndef __h3o_RuntimeGetter__
#define __h3o_RuntimeGetter__

#include "../vm/RuntimeContext.hpp"
#include "../vm/VM_Getter.hpp"
#include "../vm/glue.hpp"

namespace hydro
{

typedef object_ptr<class HGetter> hgetter;

class HGetter final : public HObject, public RuntimeContext
{
    friend HvmEnv;
    
private:
	const VM_Getter *_vgetter;
	class HProperty *_ownerProperty;
    function_glue *_glue;
    
    HGetter(HvmEnv *env, HClass *getterClass, const VM_Getter *vgetter, HProperty *ownerProperty, function_glue *glue = nullptr);
    
public:
	virtual ~HGetter();
    virtual hvalue get(HvmContext *threadContext, VM *vm, HObject *instance);
	HProperty *ownerProperty() const { return _ownerProperty; }
	virtual uint16_t modifier() const override { return _vgetter->modifier; }
	virtual std::string assemblyName() const override { return ""; }
    virtual runtime_context_type type() const override { return hvm_runtime_getter; }
};

} // namespace hydro

#endif /* __h3o_RuntimeGetter__ */
