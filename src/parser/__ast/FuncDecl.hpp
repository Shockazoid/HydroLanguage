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

#ifndef __h3o_FuncDecl__
#define __h3o_FuncDecl__

#include "ScopedDecl.hpp"
#include "Expr.hpp"
#include "Name.hpp"
#include "TypeSpec.hpp"
#include "FuncSymbol.hpp"
#include "FuncArgSymbol.hpp"

namespace hydro
{

class Farg : public Decl
{
private:
	ast_htype _type;
	ast_expr _defaultValue;
	bool _variadic;

public:
	Farg(Name *name, TypeSpec *type, Expr *defaultValue = nullptr, bool variadic = false, FuncArgSymbol *symbol = nullptr);
    //Farg(FuncArgSymbol *symbol, Expr *defaultValue = nullptr, bool variadic = false);
	virtual ~Farg();
	ast_htype type() const { return _type; }
	bool hasDefaultValue() const { return _defaultValue != nullptr; }
	ast_expr defaultValue() const { return _defaultValue; }
	bool variadic() const { return _variadic; }
};

typedef Farg *ast_farg;

class FuncArgs : public Ast
{
public:
	FuncArgs(lex_token token);
	virtual ~FuncArgs();
	void append(ast_farg arg);
	uint32_t arity() const { return numChildren(); }
};

typedef FuncArgs *ast_fargs;

class FuncDecl : public ScopedDecl
{
private:
	ast_fargs _args;
	ast_htype _retType;

public:
    int32_t slots;
	FuncDecl(Token *token, Modifier *mods, Name *name, FuncArgs *args, TypeSpec *retType, Stmt *body = nullptr, PackageSymbol *package = nullptr, FuncSymbol *symbol = nullptr);
    FuncDecl(Token *token, FuncSymbol *symbol, FuncArgs *args, TypeSpec *retType, Stmt *body = nullptr, PackageSymbol *package = nullptr);
	virtual ~FuncDecl();
	ast_fargs args() const { return _args; }
	uint32_t arity() const { return _args->arity(); }
	ast_htype returnType() const { return _retType; }
	virtual bool isUntyped() const override { return _retType == nullptr || _retType->tryCast<NilSpec>(); }
};

typedef FuncDecl *ast_func;

} // namespace hydro

#endif /* __h3o_FuncDecl__ */
