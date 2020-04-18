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

#ifndef __h3o_VM_Constructor__
#define __h3o_VM_Constructor__

#include "VM_Func.hpp"
#include "VM_Member.hpp"

namespace hydro
{

struct VM_Constructor : public VM_Func, public VM_Member
{
	virtual ~VM_Constructor() {}
};

} // namespace hydro

#endif /* __h3o_VM_Constructor__ */
