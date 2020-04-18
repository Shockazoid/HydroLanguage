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

#include "VarDecl.hpp"

namespace hydro
{

VarDecl::VarDecl(lex_token token, ast_mod mods, ast_name name, ast_htype type, ast_expr defaultValue, Symbol *symbol) : Decl{token, mods, name, symbol}, _type{type}, _defaultValue(defaultValue) {}

VarDecl::~VarDecl() {}

} // namespace hydro
