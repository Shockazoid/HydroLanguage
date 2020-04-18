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

#ifndef __h3o_Value__
#define __h3o_Value__

#include "hvalue.hpp"
#include "VM_Object.hpp"

namespace hydro
{

struct Value
{
	std::string name{""};
	const VM_Object *data{nullptr};
	hvalue content{undefined};
	bool dynamic{false};
};

} // namespace hydro

#endif /* __h3o_Value__ */
