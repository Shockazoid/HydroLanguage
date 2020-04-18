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

#ifndef __h3o_VM_Action__
#define __h3o_VM_Action__

#include "FuncData.hpp"
#include "VM_Event.hpp"

namespace hydro
{

struct VM_Action : public FuncData
{
    const VM_Event *context;
};

} // namespace hydro

#endif /* __h3o_VM_Action__ */
