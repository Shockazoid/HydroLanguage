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

#ifndef __h3o_MethodSymbol__
#define __h3o_MethodSymbol__

#include "FuncSymbol.hpp"

namespace hydro
{

class MethodSymbol : public FuncSymbol
{
public:
    MethodSymbol(Modifier *mod, Name *name, Scope *ownScope, TypeSpec *returnType) : FuncSymbol{mod, name, ownScope, returnType} {}
    virtual ~MethodSymbol() {}
};

} // namespace hydro

#endif /* __h3o_MethodSymbol__ */
