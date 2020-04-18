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

#include "NamedTupleInit.hpp"

namespace hydro
{

NamedTupleEntry::NamedTupleEntry(ast_name name, lex_token token, ast_expr value) : Ast{token}, _name{name}, _value{value}
{
	addChild(name);
	addChild(value);
}

NamedTupleEntry::~NamedTupleEntry() {}

NamedTupleInit::NamedTupleInit(lex_token token) : Literal{token} {}

NamedTupleInit::~NamedTupleInit() {}

void NamedTupleInit::append(ast_ntuple_entry entry)
{
	addChild(entry);
}

} // namespace hydro
