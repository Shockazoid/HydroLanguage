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

#ifndef __h3o_PackageDecl__
#define __h3o_PackageDecl__

#include "ScopedDecl.hpp"
#include "PackageSymbol.hpp"

namespace hydro
{

class PackageDecl : public ScopedDecl
{
public:
    PackageDecl(Token *token, PackageSymbol *symbol, Stmt *body);
    virtual ~PackageDecl();
    void append(Stmt *stmt) { addChild(stmt); }
    Name *packageName() const { return symbol()->name(); }
};

} // namespace hydro

#endif /* __h3o_PackageDecl__ */
