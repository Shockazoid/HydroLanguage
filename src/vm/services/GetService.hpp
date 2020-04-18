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

#ifndef __h3o_GetService__
#define __h3o_GetService__

#include "RuntimeService.hpp"
#include "../../system/HObject.hpp"

namespace hydro
{

struct GetService : public RuntimeService
{
    GetService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~GetService() {}
    hvalue global(std::string name) const;
    hvalue property(hobject instance, std::string name) const;
    bool property(hobject instance, std::string name, hvalue &value) const;
    hvalue index(hobject instance, hvalue key) const;
    bool index(hobject instance, hvalue key, hvalue &value) const;
    hvalue param(hobject instance, hvalue key) const;
};
    
} // namespace hydro

#endif /* __h3o_GetService__ */
