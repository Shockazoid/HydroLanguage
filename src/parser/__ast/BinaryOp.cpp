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

#include "BinaryOp.hpp"

namespace hydro
{

BinaryOp::BinaryOp(ast_expr lhs, lex_token token, ast_expr rhs) : Expr{token}, _lhs{lhs}, _rhs{rhs}
{
	addChild(_lhs);
	addChild(_rhs);
}

BinaryOp::~BinaryOp() {}

} // namespace hydro
