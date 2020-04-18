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

#ifndef __h3o_RunService__
#define __h3o_RunService__

#include <string>

#include "RuntimeService.hpp"
#include "../../parser/ParseError.hpp"
#include "../../parser/HydroCompiler.hpp"
#include "../MemorySpace.hpp"
#include "../hvalue.hpp"
#include "../Runtime.hpp"

namespace hydro
{

struct RunService : RuntimeService
{
    RunService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~RunService() {}
    hvalue hydro(Source *source, MemorySpace *scopeToUse = nullptr, Runtime *runtimeToUse = nullptr) const;
    bool hydro(Source *source, hvalue &result, MemorySpace *scopeToUse = nullptr, Runtime *runtimeToUse = nullptr) const;
    bool hydroFile(std::string filepath, hvalue &result, MemorySpace *scopeToUse = nullptr, Runtime *runtimeToUse = nullptr) const;
    hvalue hydroFile(std::string filepath, MemorySpace *scopeToUse = nullptr, Runtime *runtimeToUse = nullptr) const;
    bool hydroScript(std::string hydroCode, std::string filepath, hvalue &result, MemorySpace *scopeToUse = nullptr, Runtime *runtimeToUse = nullptr) const;
    hvalue hydroScript(std::string hydroCode, std::string filepath, MemorySpace *scopeToUse = nullptr, Runtime *runtimeToUse = nullptr) const;
};

} // namespace hydro

#endif /* __h3o_RunService__ */
