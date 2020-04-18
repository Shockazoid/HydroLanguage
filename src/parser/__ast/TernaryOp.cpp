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

#include "TernaryOp.hpp"

namespace hydro
{

TernaryOp::TernaryOp(ast_expr expr1, lex_token token1, ast_expr expr2, lex_token token2, ast_expr expr3) : Expr{token1}, _expr1{expr1}, _expr2{expr2}, _expr3{expr3}, _token2{token2} {}

TernaryOp::~TernaryOp() {}

} // namespace hydro
