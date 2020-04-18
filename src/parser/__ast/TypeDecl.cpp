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

#include "TypeDecl.hpp"

namespace hydro
{

TypeDecl::TypeDecl(lex_token token, ast_mod mods, ast_name name, ast_stmt body, PackageSymbol *package, Symbol *symbol) : ScopedDecl{token, mods, name, body, package, symbol} {}

TypeDecl::~TypeDecl() {}

} // namespace hydro
