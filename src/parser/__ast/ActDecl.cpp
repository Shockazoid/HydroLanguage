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

#include "ActDecl.hpp"

namespace hydro
{

ActDecl::ActDecl(Token *token, Modifier *mod, SimpleName *name, Name *context, Block *body, PackageSymbol *package, ActSymbol *symbol) : ScopedDecl{token, mod, name, body, package, symbol}, _context{context} {}

ActDecl::~ActDecl() {}

} // namespace hydro
