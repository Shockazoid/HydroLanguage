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

#include "ClassDecl.hpp"

namespace hydro
{

/*
ClassDecl::ClassDecl(lex_token token, ClassSymbol *symbol, ast_stmt body, PackageSymbol *package) : TypeDecl{token, symbol, body, package}, _types{}
{
}*/

ClassDecl::ClassDecl(lex_token token, ast_mod mods, ast_name name, ast_stmt body, PackageSymbol *package, ClassSymbol *symbol) : TypeDecl{token, mods, name, body, package, symbol}, _types{}
{
}

ClassDecl::~ClassDecl() {}

} // namespace hydro
