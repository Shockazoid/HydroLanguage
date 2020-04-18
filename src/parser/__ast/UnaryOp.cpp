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

#include "UnaryOp.hpp"

namespace hydro
{

UnaryOp::UnaryOp(lex_token token, ast_expr expr, bool postfix) : Expr{token}, _expr{expr}, _postfix{postfix}
{
	addChild(_expr);
}

UnaryOp::UnaryOp(ast_expr expr, lex_token token) : Expr{token}, _expr{expr}, _postfix{true}
{
	addChild(_expr);
}

UnaryOp::~UnaryOp() {}

} // namespace hydro
