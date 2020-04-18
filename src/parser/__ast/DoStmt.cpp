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

#include "DoStmt.hpp"

namespace hydro
{

DoStmt::DoStmt(lex_token token, ast_stmt body, ast_expr cond) : Stmt{token}, _body{body}, _cond{cond}
{
	addChild(body);
	addChild(cond);
}

DoStmt::~DoStmt() {}

} // namespace hydro
