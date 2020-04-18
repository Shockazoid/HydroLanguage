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

#ifndef __h3o_GCService__
#define __h3o_GCService__

#include "RuntimeService.hpp"

namespace hydro
{

struct GCService : RuntimeService
{
    GCService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~GCService() {}
    void free() const;
};

} // namespace hydro

#endif /* __h3o_GCService__ */
