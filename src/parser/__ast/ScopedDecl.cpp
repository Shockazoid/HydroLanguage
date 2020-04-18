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

#include "ScopedDecl.hpp"

namespace hydro
{

ScopedDecl::ScopedDecl(Token *token, Symbol *symbol, Stmt *body, PackageSymbol *package) : Decl{token, symbol->modifier(), symbol->name(), symbol}, _body{body}, _package{package}
{
    addChild(body);
}

ScopedDecl::ScopedDecl(Token *token, Modifier *mod, ast_name name, ast_stmt body, PackageSymbol *package, Symbol *symbol) : Decl{token, mod, name, symbol}, _body{body}, _package{package}
{
	addChild(body);
}

ScopedDecl::~ScopedDecl() {}

} // namespace hydro
