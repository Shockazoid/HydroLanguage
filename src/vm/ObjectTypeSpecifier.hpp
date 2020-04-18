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

#ifndef __h3o_ObjectTypeSpecifier__
#define __h3o_ObjectTypeSpecifier__

#include "TypeSpecifier.hpp"

namespace hydro
{
   
class ObjectTypeSpecifier : public TypeSpecifier
{
public:
    std::string assemblyName;
    ObjectTypeSpecifier(std::string asmName) : assemblyName{asmName} {}
    virtual ~ObjectTypeSpecifier() {}
    virtual bool typeCheck(const hvalue &o) override
    {
        // TODO typecheck value
        
        
        // fail
        return false;
    }
};
    
} // namespace hydro

#endif /* __h3o_ObjectTypeSpecifier__ */
