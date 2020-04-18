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

#ifndef __h3o_DictInit__
#define __h3o_DictInit__

#include "Literal.hpp"

namespace hydro
{

class DictInitEntry : public Ast
{
private:
	ast_expr _key;
	ast_expr _value;

public:
	DictInitEntry(ast_expr key, lex_token token, ast_expr value);
	virtual ~DictInitEntry();
	ast_expr key() const { return _key; }
	ast_expr value() const { return _value; }
};

typedef DictInitEntry *ast_dictentry;

class DictInit : public Literal
{
public:
	DictInit(lex_token token);
	virtual ~DictInit();
	void append(DictInitEntry *entry);
	void append(ast_expr key, lex_token token, ast_expr value);
};

typedef DictInit *ast_dict;

} // namespace hydro

#endif /* __h3o_DictInit__ */
