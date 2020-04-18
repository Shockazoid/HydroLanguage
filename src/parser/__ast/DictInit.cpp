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

#include "DictInit.hpp"

namespace hydro
{

DictInitEntry::DictInitEntry(ast_expr key, lex_token token, ast_expr value) : Ast{token}, _key{key}, _value{value} {}

DictInitEntry::~DictInitEntry() {}

DictInit::DictInit(lex_token token) : Literal{token} {}

DictInit::~DictInit() {}

void DictInit::append(DictInitEntry *entry)
{
	addChild(entry);
}

void DictInit::append(ast_expr key, lex_token token, ast_expr value)
{
	append(new DictInitEntry{key, token, value});
}

} // namespace hydro
