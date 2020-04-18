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

#ifndef __h3o_HydroTypes__
#define __h3o_HydroTypes__

#include "Ast.hpp"
#include "Expr.hpp"
#include "TypeReference.hpp"

namespace hydro
{

typedef class TypeSpec *ast_htype;

class TypeSpec : public Ast
{
protected:
	TypeSpec(lex_token token = nullptr);

public:
	virtual ~TypeSpec();
	uint32_t size() const { return numChildren(); }
};

class NilSpec final : public TypeSpec
{
public:
	NilSpec(lex_token token = nullptr) : TypeSpec{token} {}
	virtual ~NilSpec() {}
};

class ObjectSpec final : public TypeSpec
{
private:
	ast_typeref _typeRef;

public:
	ObjectSpec(ast_typeref typeRef) : TypeSpec{} { addChild(typeRef); }
	virtual ~ObjectSpec() {}
	ast_typeref typeRef() const { return _typeRef; }
};

typedef ObjectSpec *ast_objectspec;

class VariadicSpec final : public TypeSpec
{
public:
	VariadicSpec() {}
	virtual ~VariadicSpec() {}
	void append(ast_htype typeRef)
	{
		addChild(typeRef);
	}
};

typedef VariadicSpec *ast_varspec;

class TupleSpec final : public TypeSpec
{
public:
	TupleSpec(lex_token token) : TypeSpec{token} {}
	virtual ~TupleSpec() {}
	void append(ast_htype typeRef)
	{
		addChild(typeRef);
	}
};

typedef TupleSpec *ast_tuplespec;

class ArraySpec final : public TypeSpec
{
private:
	ast_htype _type;

public:
	ArraySpec(lex_token token, ast_htype type) : TypeSpec{token}, _type{type}
	{
		addChild(type);
	}
	virtual ~ArraySpec() {}
	ast_htype type() const { return _type; }
};

typedef ArraySpec *ast_arrayspec;

} // namespace hydro

#endif /* __h3o_HydroTypes__ */
