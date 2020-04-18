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

#ifndef __h3o_Process__
#define __h3o_Process__

#include "ScopedDecl.hpp"
#include "ProcessSymbol.hpp"
#include "Block.hpp"

namespace hydro
{

class Process : public ScopedDecl
{
    Process(Token *token, Modifier *mod, SimpleName *name, Block *body = nullptr, PackageSymbol *package = nullptr, ProcessSymbol *symbol = nullptr);
    virtual ~Process();
};

} // namespace hydro

#endif /* __h3o_ProcessCxt__ */
