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

#include "HEventContext.hpp"
#include "HObjectContext.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

HEventContext::HEventContext(HvmEnv *env, HClass *eventContextClass, const EventData *vevent, HEventContext *superEvent) : HObject{env, eventContextClass}, _vevent{vevent}, _superEvent{superEvent}
{
    _cxt->ensureCapacity(_cxt->_size + _vevent->netypes);
    uint16_t j = _cxt->_size;

    for(uint16_t i = 0; i < _vevent->netypes; i++)
    {
        _cxt->fields[j] = new HObjectContext::property_space{};
        _cxt->fields[j]->key = _vevent->etypes[i]->name;
        _cxt->fields[j]->dynamic = false;
        _cxt->fields[j]->data = _vevent->etypes[i];
        _cxt->fields[j++]->value = _env->createEtype(_vevent->etypes[i], this);
    }
    
    _cxt->_size = j;
}

HEventContext::~HEventContext() {}
        
} // namespace hydro
