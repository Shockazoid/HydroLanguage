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

#include "FuncExpr.hpp"

namespace hydro
{

FuncExpr::FuncExpr(Token *token, Modifier *mods, FuncArgs *args, TypeSpec *retType, Block *body) : _mods{mods}, _args{args}, _body{body} {}

FuncExpr::~FuncExpr() {}

} // namespace hydro
