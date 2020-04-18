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

#ifndef __h3o_LogService__
#define __h3o_LogService__

#include <string>

#include "RuntimeService.hpp"
#include "../hvalue.hpp"

namespace hydro
{

struct LogService : RuntimeService
{
    LogService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~LogService() {}
    void print(hvalue value) const;
    void error(hvalue error) const;
    void clear() const;
};

} // namespace hydro

#endif /* __h3o_LogService__ */
