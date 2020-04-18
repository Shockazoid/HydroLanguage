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

#include "SetterDecl.hpp"

namespace hydro
{

SetterDecl::SetterDecl(lex_token token, Modifier *mods, FuncArgs *args, Block *body) : FuncDecl{token, mods, nullptr, args, nullptr, body} {}

SetterDecl::~SetterDecl() {}

} // namespace hydro
