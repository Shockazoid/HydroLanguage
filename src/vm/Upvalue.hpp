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

#ifndef __h3o_Upvalue__
#define __h3o_Upvalue__

#include "Value.hpp"

namespace hydro
{

struct Upvalue
{
	void *object;
	Value *slot;
};

} // namespace hydro

#endif /* __h3o_Upvalue__ */
