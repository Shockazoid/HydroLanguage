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

#include "HContext.hpp"

namespace hydro
{

HContext::HContext(HvmEnv *env, HClass *contextClass, const ContextData *vcontext) : HObject{env, contextClass}, _vcontext{vcontext} {}

HContext::~HContext() {}

void HContext::setup()
{
    // abstract
}

} // namespace hydro
