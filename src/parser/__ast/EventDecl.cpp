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

#include "EventDecl.hpp"

namespace hydro
{

EventDecl::EventDecl(lex_token token, Modifier *mods, Name *name, Block *body, EventSymbol *symbol, PackageSymbol *package) : TypeDecl{token, mods, name, body, package, symbol} {}

EventDecl::~EventDecl() {}

} // namespace hydro
