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

#include "JsonInit.hpp"

namespace hydro
{

JsonInitProperty::JsonInitProperty(ast_literal key, lex_token token, ast_expr value) : Ast{token}, _key{key}, _value{value} {}

JsonInitProperty::~JsonInitProperty() {}

JsonInit::JsonInit(lex_token token) : Literal{token} {}

JsonInit::~JsonInit() {}

void JsonInit::append(ast_jsonProp prop)
{
	addChild(prop);
}

} // namespace hydro
