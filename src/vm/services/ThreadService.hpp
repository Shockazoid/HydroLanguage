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

#ifndef __h3o_ThreadService__
#define __h3o_ThreadService__

#include "RuntimeService.hpp"
#include "../HvmContext.hpp"

namespace hydro
{

struct ThreadService : RuntimeService
{
    ThreadService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~ThreadService() {}
    HvmContext *main() const;
    HvmContext *current() const;
    void run(hvalue runnable);
    template <typename ReturnType, typename ...Args>
    void run(ReturnType (*fn)(Args...)) {}
};

} // namespace hydro

#endif /* __h3o_ThreadService__ */
