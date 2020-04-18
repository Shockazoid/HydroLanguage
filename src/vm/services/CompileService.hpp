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

#ifndef __h3o_CompileService__
#define __h3o_CompileService__

#include "RuntimeService.hpp"

namespace hydro
{

struct CompileService : RuntimeService
{
    CompileService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~CompileService() {}
    void aot() const {}
    void jit() const {}
};

} // namespace hydro

#endif /* CompileService_hpp */
