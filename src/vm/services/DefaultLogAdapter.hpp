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

#ifndef __h3o_DefaultLogAdapter__
#define __h3o_DefaultLogAdapter__

#include <iostream>

#include "LogAdapter.hpp"

namespace hydro
{

class DefaultLogAdapter
{
public:
    virtual print(HvmEnv *env, hvalue value) const override { std::cout << env->cast()->string(hvalue) << std::endl; }
    virtual error(HvmEnv *env, hvalue value) const override { std::cout << env->cast()->string(hvalue) << std::endl; }
    virtual clear(HvmEnv *env) const {}
};

} // namespace hydro

#endif /* __h3o_DefaultLogAdapter__ */
