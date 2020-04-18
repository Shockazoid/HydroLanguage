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

#include "ParamDecl.hpp"

namespace hydro
{

ParamDecl::ParamDecl(Token *token, Modifier *mod, Name *name, TypeSpec *type, ParamSymbol *symbol, Expr *defaultValue) : Decl{token, mod, name, symbol}, _type{type}, _defaultValue{defaultValue} {}

ParamDecl::~ParamDecl() {}

} // namespace hydro
