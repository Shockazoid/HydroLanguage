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

#ifndef __h3o_VM_Getter__
#define __h3o_VM_Getter__

#include "VM_Method.hpp"

namespace hydro
{

struct VM_Getter : public VM_Method
{
    bool automatic{false};
	struct VM_Property *property{nullptr};
};

} // namespace hydro

#endif /* __h3o_VM_Getter__ */
