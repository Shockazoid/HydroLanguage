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

#include "ExprStmt.hpp"

namespace hydro
{

ExprStmt::ExprStmt(ast_expr expr) : Stmt{}, _expr{expr}
{
	addChild(expr);
}

ExprStmt::~ExprStmt() {}

} // namespace hydro
