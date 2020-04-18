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

#include "ReturnStmt.hpp"

namespace hydro
{

ReturnStmt::ReturnStmt(lex_token token, Expr *expr) : Stmt{token}, _expr{expr}
{
	addChild(expr);
}

ReturnStmt::~ReturnStmt() {}

} // namespace hydro
