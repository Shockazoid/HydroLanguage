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

#ifndef __h3o_TypingService__
#define __h3o_TypingService__

#include <string>

#include "RuntimeService.hpp"
#include "../hvalue.hpp"
#include "../../system/HObject.hpp"
#include "../../system/HClass.hpp"
#include "../../system/HObject.hpp"

namespace hydro
{

struct TypingService : RuntimeService
{
    TypingService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~TypingService() {}
    bool check(hobject type, hvalue instance) const;
    bool check(std::string qualifiedName, hvalue instance) const;
};

} // namespace hydro

#endif /* __h3o_TypingService__ */
