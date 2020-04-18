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

#ifndef __h3o_VM_Event__
#define __h3o_VM_Event__

#include "VM_Object.hpp"
#include "VM_Member.hpp"
#include "VM_Etype.hpp"
#include "EventParam.hpp"

namespace hydro
{

struct VM_Event : public VM_Object
{
    const VM_Event *base;
	VM_Etype **etypes;
    uint16_t netypes;
    uint16_t nparams;
    EventParam **params;
};

} // namespace hydro

#endif /* __h3o_VM_Event__ */
