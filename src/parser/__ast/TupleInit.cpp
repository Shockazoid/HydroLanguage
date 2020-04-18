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

#include "TupleInit.hpp"

namespace hydro
{

TupleInit::TupleInit(lex_token token) : Literal{token} {}

TupleInit::~TupleInit() {}

void TupleInit::append(ast_expr expr)
{
	addChild(expr);
}

} // namespace hydro
