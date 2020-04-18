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

#ifndef __h3o_EventService__
#define __h3o_EventService__

#include "RuntimeService.hpp"
#include "../hvalue.hpp"
#include "../../system/HEventContext.hpp"
#include "../../system/HEventType.hpp"

namespace hydro
{

struct EventService : RuntimeService
{
    EventService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~EventService() {}
    void haltPropagation() const;
    void haltImmediatePropagation() const;
    hetype type() const;
    hetype etype(hevent event, std::string etypeName) const;
};

} // namespace hydro

#endif /* __h3o_EventService__ */
