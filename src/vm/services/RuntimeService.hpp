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

#ifndef __h3o_RuntimeService__
#define __h3o_RuntimeService__

#include <string>

#include "../MemorySpace.hpp"
#include "../HvmContext.hpp"
#include "../../system/HObjectContext.hpp"

namespace hydro
{

struct RuntimeService
{
protected:
    class HvmEnv *_env;
    class VM *_vm;
    MemorySpace *_globals;
    const bool &_debug;
    HvmContext *&_currentContext;
    HvmContext *&_mainContext;
    const std::string &_sdkPath;
    const std::string &_srcPath;
    const std::string &_libPath;

    RuntimeService(HvmEnv *env);
    
    HObjectContext *objectContext(hobject obj) const;
    
public:
    virtual ~RuntimeService() {}
};

} // namespace hydro

#endif /* __h3o_RuntimeService__ */
