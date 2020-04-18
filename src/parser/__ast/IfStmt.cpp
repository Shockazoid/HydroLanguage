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

#include "IfStmt.hpp"

namespace hydro
{

ElseStmt::ElseStmt(lex_token token, ast_stmt body) : Stmt{token}, _body{body}
{
	addChild(body);
}

ElseStmt::~ElseStmt() {}

IfStmt::IfStmt(lex_token token, ast_expr cond, ast_stmt body, ast_else elseStmt) : Stmt{token}, _cond{cond}, _body{body}, _elseStmt{elseStmt}
{
	addChild(cond);
	addChild(body);
	addChild(elseStmt);
}

IfStmt::~IfStmt() {}

} // namespace hydro
