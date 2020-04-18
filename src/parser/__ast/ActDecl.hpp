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

#ifndef __h3o_Act__
#define __h3o_Act__

#include "ScopedDecl.hpp"
#include "ActSymbol.hpp"
#include "Block.hpp"

namespace hydro
{

class ActDecl : public ScopedDecl
{
private:
    Name *_context;
    
public:
    ActDecl(Token *token, Modifier *mod, SimpleName *name, Name *context, Block *body = nullptr, PackageSymbol *package = nullptr, ActSymbol *symbol = nullptr);
    virtual ~ActDecl();
    Name *context() const { return _context; }
};

} // namespace hydro

#endif /* __h3o_Act__ */
