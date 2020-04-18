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

#include "StringLibrary.hpp"

namespace hydro
{

void StringLibrary::appendScriptFromString(std::string hydroCode, std::string relativePath, std::string qname)
{
    hydro_script *script = new hydro_script{};
    script->code = hydroCode;
    script->path = relativePath;
    script->qname = qname;
    m_scripts.push_back(script);
}

Source *StringLibrary::tryResolve(std::string qname) const
{
    for (hydro_script *script : m_scripts)
        if(script->qname == qname)
            return new StringSource{script->code, script->path};
    
    // fail
    return nullptr;
}

} // namespace hydro
