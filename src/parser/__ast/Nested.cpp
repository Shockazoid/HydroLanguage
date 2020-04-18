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

#include "Nested.hpp"

namespace hydro
{

Nested::Nested(lex_token token, ast_expr expr) : Expr{token}, _expr{expr}
{
	addChild(_expr);
}

Nested::Nested(ast_expr expr) : Expr{}, _expr{expr}
{
	addChild(_expr);
}

Nested::~Nested() {}

} // namespace hydro
