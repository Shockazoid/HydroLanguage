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

#ifndef __h3o_EventContext__
#define __h3o_EventContext__

#include "HObject.hpp"
#include "HEventType.hpp"
#include "../vm/EventData.hpp"

namespace hydro
{

class HEventContext : public HObject
{
    friend class HvmEnv;
    
private:
    const EventData *_vevent;
    HEventContext *_superEvent;
    HEventContext(HvmEnv *env, HClass *eventContextClass, const EventData *vevent, HEventContext *superEvent = nullptr);
    
public:
    virtual ~HEventContext();
    const EventData *vevent() { return _vevent; }
    HEventContext *superEvent() const { return _superEvent; }
    virtual std::string toString() override { return "[event " + get_simple_name(_vevent->name) + "]"; }
};

typedef object_ptr<HEventContext> hevent;

} // namespace hydro

#endif /* __h3o_Event__ */
