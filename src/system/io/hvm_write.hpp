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

#ifndef __h3o_hvm_write__
#define __h3o_hvm_write__

#include <iostream>

#include "../HString.hpp"
#include "../../vm/HvmEnv.hpp"

namespace hydro
{

void hvm_write(HvmEnv *env, hstring val);

} // namespace hydro


#endif /* __h3o_hvm_write__ */
