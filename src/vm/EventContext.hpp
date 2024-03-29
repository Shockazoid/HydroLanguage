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

#include "VM_Event.hpp"
#include "Value.hpp"

namespace hydro
{

struct EventContext
{
    const VM_Event *data;
    Value **params;
};

} // namespace hydro

#endif /* __h3o_EventContext__ */
