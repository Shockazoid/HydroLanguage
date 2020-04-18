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

#include "ConstructorDecl.hpp"

namespace hydro
{

ConstructorDecl::ConstructorDecl(lex_token token, ast_mod mods, ast_fargs fargs, ast_htype returnType, ast_stmt block, ConstructorSymbol *symbol) : FuncDecl{token, mods, nullptr, fargs, returnType, block, nullptr, symbol} {}

ConstructorDecl::~ConstructorDecl() {}

} // namespace hydro
