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

#include "HState.hpp"
#include "HvmEnv.hpp"

namespace hydro
{

HState::HState(HvmEnv *env, HClass *stateClass, HObject *stateContext) : HObject{env, stateClass}, _stateContext{stateContext} {}

HState::~HState() {}

} // namespace hydro
