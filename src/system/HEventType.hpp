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

#ifndef __h3o_EventType__
#define __h3o_EventType__

#include "HObject.hpp"
#include "../vm/EtypeData.hpp"

namespace hydro
{

class HEventType : public HObject
{
    friend class HvmEnv;
    
private:
    const EtypeData *_vetype;
    class HEventContext *_owner;
    std::string _value;
    HEventType(HvmEnv *env, HClass *eventTypeClass, const EtypeData *vetype, HEventContext *owner);
    
public:
    virtual ~HEventType();
    HEventContext *owner() const { return _owner; }
    virtual std::string toString() override { return "[etype " + _value + "]"; }
};

typedef object_ptr<HEventType> hetype;

} // namespace hydro

#endif /* __h3o_EventType__ */
