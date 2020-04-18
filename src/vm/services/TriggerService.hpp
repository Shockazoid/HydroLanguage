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

#ifndef __h3o_TriggerService__
#define __h3o_TriggerService__

#include "RuntimeService.hpp"

namespace hydro
{

struct TriggerService : RuntimeService
{
    TriggerService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~TriggerService() {}
};

} // namespace hydro

#endif /* TriggerService_hpp */
