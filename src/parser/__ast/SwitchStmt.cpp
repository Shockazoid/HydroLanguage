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

#include "SwitchStmt.hpp"

namespace hydro
{

CaseClause::CaseClause(lex_token token, ast_expr expr) : Stmt{token}, _expr{expr}
{
	addChild(expr);
}

CaseClause::~CaseClause() {}

DefaultClause::DefaultClause(lex_token token) : Stmt{token} {}

DefaultClause::~DefaultClause() {}

SwitchStmt::SwitchStmt(lex_token token, ast_expr expr, ast_stmt body) : Stmt{token}, _expr{expr}, _body{body}
{
	addChild(expr);
}

SwitchStmt::~SwitchStmt() {}

} // namespace hydro
