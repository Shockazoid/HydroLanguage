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

#include "PropertyDecl.hpp"

namespace hydro
{

PropertyDecl::PropertyDecl(lex_token token, ast_mod mods, ast_name name, ast_htype type, ast_expr defaultValue, PropertySymbol *symbol) : Decl{token, mods, name, symbol}, _type{type}, _defaultValue{defaultValue}, _body{nullptr}
{
	addChild(defaultValue);
}

PropertyDecl::PropertyDecl(lex_token token, ast_mod mods, ast_name name, ast_htype type, Stmt *body, PropertySymbol *symbol)
    : Decl{token, mods, name, symbol}, _type{type}, _defaultValue{nullptr}, _body{body}
{
	addChild(body);
}

PropertyDecl::~PropertyDecl() {}

} // namespace hydro
