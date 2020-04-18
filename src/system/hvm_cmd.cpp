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

#include "hvm_cmd.hpp"

namespace hydro
{

int hvm_cmd(HvmEnv *env, hstring command)
{
    std::string commandString = env->cast()->string(command);
    return std::system(commandString.c_str());
}

} // namespace hydro
