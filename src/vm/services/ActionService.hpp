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

#ifndef __h3o_ActionService__
#define __h3o_ActionService__

#include "RuntimeService.hpp"
#include "../../system/HEventContext.hpp"

namespace hydro
{

struct ActionService : RuntimeService
{
    ActionService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~ActionService() {}
    hevent context() const;
};

} // namespace hydro

#endif /* __h3o_ActionService__ */
