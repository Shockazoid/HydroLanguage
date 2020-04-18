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

#ifndef __h3o_JsonInit__
#define __h3o_JsonInit__

#include "Literal.hpp"

namespace hydro
{

class JsonInitProperty : public Ast
{
private:
	ast_literal _key;
	ast_expr _value;

public:
	JsonInitProperty(ast_literal key, lex_token token, ast_expr value);
	virtual ~JsonInitProperty();
	ast_literal key() const { return _key; }
	ast_expr value() const { return _value; }
};

typedef JsonInitProperty *ast_jsonProp;

class JsonInit : public Literal
{
public:
	JsonInit(lex_token token);
	virtual ~JsonInit();
	void append(ast_jsonProp prop);
};

typedef JsonInit *ast_json;

} // namespace hydro

#endif /* __h3o_JsonInit__ */
