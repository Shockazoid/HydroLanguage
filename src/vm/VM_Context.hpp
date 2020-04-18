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

#ifndef __h3o_VM_Context__
#define __h3o_VM_Context__

#include "VM_Object.hpp"
#include "EventParam.hpp"

namespace hydro
{

struct VM_Context : public VM_Object
{
    VM_Context *base;
    uint16_t nparams;
    EventParam **params;
};

} // namespace hydro

#endif /* __h3o_VM_Context__ */
