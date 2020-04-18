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

#include "HSetter.hpp"
#include "HProperty.hpp"
#include "HvmEnv.hpp"

namespace hydro
{

HSetter::HSetter(HvmEnv *env, HClass *setterClass, const VM_Setter *vsetter, HProperty *ownerProperty, function_glue *glue) : HObject{env, setterClass}, RuntimeContext{ownerProperty->ownerClass()}, _vsetter{vsetter}, _glue{glue} {}

HSetter::~HSetter() {}

void HSetter::set(HvmContext *threadContext, VM *vm, HObject *instance, const hvalue value)
{
    if(!_glue)
    {
        // auto property
        auto fields = instance->_cxt->fields;
        uint32_t n = instance->_cxt->_size;
        
        for(uint32_t i = 0; i < n; i++)
        {
            if(fields[i]->data == _vsetter->property)
            {
                fields[i]->value = value;
                return;
            }
        }
        
        // fail
        return;
    }
    
    std::list<hvalue> args;
    args.push_back(value);
    _glue->call(threadContext, this, vm, args, instance);
}


} // namespace hydro
