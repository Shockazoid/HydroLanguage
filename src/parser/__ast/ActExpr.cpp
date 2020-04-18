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

#include "ActExpr.hpp"

namespace hydro
{

ActExpr::ActExpr(Token *token, Modifier *mods, Name *context, Block *body) : Expr{token}, _mods{mods}, _context{context}, _body{body} {}

ActExpr::~ActExpr() {}

} // namespace hydro
