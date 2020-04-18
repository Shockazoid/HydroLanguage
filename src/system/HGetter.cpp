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

#include "HGetter.hpp"
#include "HProperty.hpp"

namespace hydro
{

HGetter::HGetter(HvmEnv *env, HClass *getterClass, const VM_Getter *vgetter, HProperty *ownerProperty, function_glue *glue) : HObject{env, getterClass}, RuntimeContext{ownerProperty->ownerClass()}, _vgetter{vgetter}, _glue{glue} {}

HGetter::~HGetter() {}

hvalue HGetter::get(HvmContext *threadContext, VM *vm, HObject *instance)
{
    if(!_glue)
    {
        // auto property
        auto fields = instance->_cxt->fields;
        uint32_t n = instance->_cxt->_size;
        
        for(uint32_t i = 0; i < n; i++)
        {
            if(fields[i]->data == _vgetter->property)
            {
                return fields[i]->value;
            }
        }
        
        // fail
        return undefined;
    }
    
    std::list<hvalue> args;
    return _glue->call(threadContext, this, vm, args, instance);
}

} // namespace hydro
