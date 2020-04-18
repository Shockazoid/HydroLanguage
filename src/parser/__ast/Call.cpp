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

#include "Call.hpp"

namespace hydro
{

Call::Call(ast_expr lhs, lex_token token, ast_cargs args) : BinaryOp{lhs, token, args}, _args{args} {}

Call::~Call() {}

} // namespace hydro
