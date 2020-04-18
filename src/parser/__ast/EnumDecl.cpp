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

#include "EnumDecl.hpp"

namespace hydro
{

EnumDecl::EnumDecl(lex_token token, Modifier *mods, Name *name) : Decl{token, mods, name} {}

EnumDecl::~EnumDecl() {}

} // namespace hydro
