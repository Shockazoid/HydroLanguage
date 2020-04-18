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

#ifndef __h3o_ThrowService__
#define __h3o_ThrowService__

#include "RuntimeService.hpp"
#include "../hvalue.hpp"

namespace hydro
{

struct ThrowService : RuntimeService
{
    ThrowService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~ThrowService() {}
    void target(hvalue err) const;
    void error(std::string text, int32_t code = 0) const;
    void argumentError(std::string text, int32_t code = 0) const;
    void nullPointerError(std::string text, int32_t code = 0) const;
    void boundsError(std::string text, int32_t code = 0) const;
    void ioError(std::string text, int32_t code = 0) const;
    void securityError(std::string text, int32_t code = 0) const;
    bool errorThrown() const;
};

} // namespace hydro

#endif /* __h3o_ThrowService__ */
