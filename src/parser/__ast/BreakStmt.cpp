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

#include "BreakStmt.hpp"

namespace hydro
{

BreakStmt::BreakStmt(lex_token token, ast_expr expr) : Stmt{token}, _expr{expr} {}

BreakStmt::~BreakStmt() {}

} // namespace hydro
