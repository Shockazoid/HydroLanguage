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
#include "../vm/VM_Event.hpp"

namespace hydro
{

class HEventContext : public HObject
{
    friend class HvmEnv;
    
private:
    const VM_Event *_vevent;
    HEventContext *_superEvent;
    HEventContext(HvmEnv *env, HClass *eventContextClass, const VM_Event *vevent, HEventContext *superEvent = nullptr);
    
public:
    virtual ~HEventContext();
    const VM_Event *vevent() { return _vevent; }
    HEventContext *superEvent() const { return _superEvent; }
    virtual std::string toString() override { return "[event " + get_simple_name(_vevent->name) + "]"; }
};

typedef object_ptr<HEventContext> hevent;

} // namespace hydro

#endif /* __h3o_Event__ */
