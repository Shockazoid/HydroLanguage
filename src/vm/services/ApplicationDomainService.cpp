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

#include "ApplicationDomainService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

Runtime *ApplicationDomainService::system() const { return _system; }

Runtime *ApplicationDomainService::current() const
{
    HvmContext *cxt = _env->thread()->current();
    
    if(cxt && cxt->currentFrame)
        return cxt->currentFrame->chunk->runtime;

    // default
    return _system;
}

} // namespace hydro
