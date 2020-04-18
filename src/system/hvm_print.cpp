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

#include "hvm_print.hpp"

namespace hydro
{

void hvm_print(HvmEnv *env, hvalue value)
{
    std::cout << env->cast()->string(value) << std::endl; // TODO use adapter to output values such that standard i/o can be used or a custom tracer
}

} // namespace hydro
