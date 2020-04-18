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

#ifndef __h3o_LogAdapter__
#define __h3o_LogAdapter__

#include "../HvmEnv.hpp"
#include "../hvalue.hpp"

namespace hydro
{

class LogAdapter
{
public:
    virtual void print(HvmEnv *env, hvalue value) const = 0;
    virtual void error(HvmEnv *env, hvalue value) const = 0;
    virtual void clear(HvmEnv *env) const = 0;
};

} // namespace hydro

#endif /* __h3o_LogAdapter__ */
