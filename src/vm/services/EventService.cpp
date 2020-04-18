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

#include "EventService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

void EventService::haltPropagation() const
{
    
}

void EventService::haltImmediatePropagation() const
{
    
}

hetype EventService::type() const
{
    
    return nullptr;
}


hetype EventService::etype(hevent event, std::string etypeName) const
{
    hvalue result;
    
    if(_env->get()->property(event, etypeName, result))
    {
        if(is_etype(result))
            return result;
    }
    
    // fail
    return nullptr;
}

} // namespace hydro
