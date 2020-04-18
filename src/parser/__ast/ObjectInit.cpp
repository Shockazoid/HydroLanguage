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

#include "ObjectInit.hpp"

namespace hydro
{

ObjectInitProperty::ObjectInitProperty(ast_name name, lex_token token, ast_expr value) : Ast{token}, _name{name}, _value{value} {}

ObjectInitProperty::~ObjectInitProperty() {}

ObjectInit::ObjectInit(lex_token token) : Literal{token} {}

ObjectInit::~ObjectInit() {}

void ObjectInit::append(ast_objectProp property) { addChild(property); }

void ObjectInit::append(ast_name name, lex_token token, ast_expr value) { addChild(new ObjectInitProperty{name, token, value}); }

} // namespace hydro
