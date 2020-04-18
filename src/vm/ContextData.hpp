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

#ifndef __h3o_ContextData__
#define __h3o_ContextData__

#include "VM_Object.hpp"
#include "EventParam.hpp"

namespace hydro
{

struct ContextData : public VM_Object
{
    ContextData *base;
    uint16_t nparams;
    EventParam **params;
};

} // namespace hydro

#endif /* __h3o_ContextData__ */
