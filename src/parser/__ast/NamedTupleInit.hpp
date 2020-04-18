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

#ifndef __h3o_NamedTupleInit__
#define __h3o_NamedTupleInit__

#include "Literal.hpp"
#include "Name.hpp"

namespace hydro
{

class NamedTupleEntry : public Ast
{
private:
	ast_name _name;
	ast_expr _value;

public:
	NamedTupleEntry(ast_name name, lex_token token, ast_expr value);
	virtual ~NamedTupleEntry();
	ast_name name() const { return _name; }
	ast_expr value() const { return _value; }
};

typedef NamedTupleEntry *ast_ntuple_entry;

class NamedTupleInit : public Literal
{
public:
	NamedTupleInit(lex_token token);
	virtual ~NamedTupleInit();
	void append(ast_ntuple_entry entry);
};

typedef NamedTupleInit *ast_ntuple;

} // namespace hydro

#endif /* __h3o_NamedTupledInit__ */
