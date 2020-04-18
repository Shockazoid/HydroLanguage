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

#include "HProperty.hpp"
#include "HvmEnv.hpp"

namespace hydro
{

HProperty::HProperty(HvmEnv *env, HClass *packageClass, const VM_Property *vprop, HClass *ownerClass, property_glue *glue) : HObject{env, packageClass}, RuntimeContext{ownerClass}, _vprop{vprop}, _glue{glue}
{
    // setup
    _getter = _vprop->getter ? _env->createGetter(_vprop->getter, this) : nullptr;
    _setter = _vprop->setter ? _env->createSetter(_vprop->setter, this) : nullptr;
}

HProperty::~HProperty() {}

bool HProperty::get(HvmContext *threadContext, VM *vm, hvalue instance, hvalue &value)
{
	if(_getter)
    {
        value = _getter->get(threadContext, vm, instance);
        return true;
    }
    else if (_glue)
    {
        // property is not write-only
        return _glue->get(threadContext, this, vm, instance, value);
    }
    
    // fail
	return false;
}

bool HProperty::set(HvmContext *threadContext, VM *vm, hvalue instance, const hvalue &value)
{
    if(_setter)
    {
        _setter->set(threadContext, vm, instance, value);
        return true;
    }
    else if(_glue)
    {
        // property is not read-only
        hvalue val = value; // remove reference
        return _glue->set(threadContext, this, vm, instance, val);
    }
    
    // fail
    return false;
}

} // namespace hydro
