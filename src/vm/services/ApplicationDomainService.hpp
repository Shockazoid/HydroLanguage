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

#ifndef __h3o_ApplicationDomainService__
#define __h3o_ApplicationDomainService__

#include "RuntimeService.hpp"
#include "../Runtime.hpp"

namespace hydro
{

struct ApplicationDomainService : RuntimeService
{
private:
    Runtime *_system;
    
public:
    ApplicationDomainService(HvmEnv *env) : RuntimeService{env}, _system{new Runtime{}} {}
    virtual ~ApplicationDomainService() {}
    Runtime *system() const;
    Runtime *current() const;
};

} // namespace hydro

#endif /* __h3o_ApplicationDomainService__ */
