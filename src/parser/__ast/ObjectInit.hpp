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

#ifndef __h3o_ObjectInit__
#define __h3o_ObjectInit__

#include "Literal.hpp"
#include "Name.hpp"

namespace hydro
{

class ObjectInitProperty : public Ast
{
private:
	ast_name _name;
	ast_expr _value;

public:
	ObjectInitProperty(ast_name name, lex_token token, ast_expr value);
	virtual ~ObjectInitProperty();
	ast_name name() const { return _name; }
	ast_expr value() const { return _value; }
};

typedef ObjectInitProperty *ast_objectProp;

class ObjectInit : public Literal
{
public:
	ObjectInit(lex_token token);
	virtual ~ObjectInit();
	void append(ast_objectProp property);
	void append(ast_name name, lex_token token, ast_expr value);
};

typedef ObjectInit *ast_object;

} // namespace hydro

#endif /* __h3o_ObjectInit__ */
