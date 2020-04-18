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

#ifndef __h3o_LocalVar__
#define __h3o_LocalVar__

#include <string>
#include <cstdlib>

#include "VM_Object.hpp"
#include "TypeSpecifier.hpp"

namespace hydro
{

struct LocalVar : public VM_Object
{
	TypeSpecifier *type{nullptr};
	virtual ~LocalVar() {}
};
} // namespace hydro

#endif /* __h3o_LocalVar__ */
