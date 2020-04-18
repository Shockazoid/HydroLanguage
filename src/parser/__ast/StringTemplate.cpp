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

#include "StringTemplate.hpp"

namespace hydro
{

StringTemplate::StringTemplate(lex_token token) : Literal{token} {}

StringTemplate::~StringTemplate() {}

void StringTemplate::append(ast_expr expr)
{
	addChild(expr);
}

} // namespace hydro
