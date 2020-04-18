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

#include "ForStmt.hpp"

namespace hydro
{

ForStmt::ForStmt(lex_token token, ast_expr init, ast_expr cond, ast_expr it, ast_stmt body) : _init{init}, _cond{cond}, _it{it}, _body{body}
{
	addChild(init);
	addChild(cond);
	addChild(it);
	addChild(body);
}

ForStmt::~ForStmt() {}

} // namespace hydro
