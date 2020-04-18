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

#include "Decl.hpp"

namespace hydro
{

Decl::Decl(Token *token, Symbol *symbol) : Stmt{token, symbol} {}

Decl::Decl(lex_token token, ast_mod mods, ast_name name, Symbol *symbol) : Stmt{token, symbol}, _mods{mods}, _name{name} {}

Decl::~Decl() {}

} // namespace hydro
