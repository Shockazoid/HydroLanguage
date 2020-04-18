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

#ifndef __h3o_UtilityService__
#define __h3o_UtilityService__

#include <string>

#include "RuntimeService.hpp"
#include "../hvalue.hpp"

namespace hydro
{

struct UtilityService : RuntimeService
{
    UtilityService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~UtilityService() {}
    std::string qualifiedName(const hvalue &value) const;
    std::string describe(const hvalue &value) const;
};

} // namespace hydro

#endif /* __h3o_UtilityService__ */
