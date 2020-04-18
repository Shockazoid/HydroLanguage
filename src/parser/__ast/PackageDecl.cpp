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

#include "PackageDecl.hpp"

namespace hydro
{

PackageDecl::PackageDecl(Token *token, PackageSymbol *symbol, Stmt *body) : ScopedDecl{token, symbol, body} {}

PackageDecl::~PackageDecl() {}

} // namespace hydro
