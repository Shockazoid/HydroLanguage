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

#include "ThreadService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

HvmContext *ThreadService::main() const { return _env->_mainContext; }


HvmContext *ThreadService::current() const { return _env->_currentContext; }

void ThreadService::run(hvalue runnable) {}

} // namespace hydro
