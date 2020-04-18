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

#include "ForEachStmt.hpp"

namespace hydro
{

ForEachStmt::ForEachStmt(lex_token token, ast_name valueName, ast_expr expr, ast_stmt body) : Stmt{token}, _keyName{}, _valueName{valueName}, _expr{expr}, _body{body}
{
	addChild(body);
	addChild(expr);
}

ForEachStmt::ForEachStmt(lex_token token, ast_name keyName, ast_name valueName, ast_expr expr, ast_stmt body) : Stmt{token}, _keyName{keyName}, _valueName{valueName}, _expr{expr}, _body{body}
{
	addChild(expr);
	addChild(body);
}

ForEachStmt::~ForEachStmt() {}

} // namespace hydro
