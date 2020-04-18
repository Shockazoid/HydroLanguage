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

#ifndef __h3o_hvm_eval__
#define __h3o_hvm_eval__

#include "../vm/hvalue.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

hvalue hvm_eval(class HvmEnv *env, class HvmContext *cxt, hvalue hydroCode, std::string scope);

} // namespace hydro

#endif /* __h3o_hvm_eval__ */
