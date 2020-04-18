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

#ifndef __h3o_EvalService__
#define __h3o_EvalService__

#include "RuntimeService.hpp"
#include "../Runtime.hpp"
#include "../hvalue.hpp"
#include "../MemorySpace.hpp"
#include "../../parser/Source.hpp"
#include "../../parser/HydroCompiler.hpp"

namespace hydro
{

struct EvalService : RuntimeService
{
    EvalService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~EvalService() {}
    bool code(std::string hydroCode, hvalue &result, MemorySpace *scopeToUse = nullptr) const;
    hvalue code(std::string hydroCode, MemorySpace *scopeToUse = nullptr) const;
    bool source(Source *src, hvalue &result, MemorySpace *scopeToUse = nullptr) const;
    hvalue source(Source *src, MemorySpace *scopeToUse = nullptr) const;
};

} // namespace hydro

#endif /* __h3o_EvalService__ */
