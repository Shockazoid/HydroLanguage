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

#ifndef __h3o_LibraryDecl__
#define __h3o_LibraryDecl__

#include "ScopedDecl.hpp"
#include "LibrarySymbol.hpp"

namespace hydro
{

class LibraryDecl : public ScopedDecl
{
private:
    
public:
    LibraryDecl(Token *token, LibrarySymbol *symbol, Stmt *body);
    virtual ~LibraryDecl();
    void append(Stmt *stmt) { addChild(stmt); }
};

} // namespace hydro

#endif /* __h3o_LibraryDecl__ */
