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

#include "HEventType.hpp"

namespace hydro
{

HEventType::HEventType(HvmEnv *env, HClass *eventTypeClass, const VM_Etype *vetype, HEventContext *owner) : HObject{env, eventTypeClass}, _vetype{vetype}, _owner{owner}
{
    _value = vetype->name;
}

HEventType::~HEventType() {}

} // namespace hydro
