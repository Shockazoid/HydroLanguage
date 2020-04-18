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

#include "FuncDecl.hpp"

namespace hydro
{

Farg::Farg(ast_name name, ast_htype type, ast_expr defaultValue, bool variadic, FuncArgSymbol *symbol) : Decl{name->token(), nullptr, name, symbol}, _type{type}, _defaultValue{defaultValue}, _variadic{variadic} {}

//Farg::Farg(FuncArgSymbol *symbol, ast_expr defaultValue, bool variadic) : Decl{symbol->token(), symbol}, _type{symbol->type()}, _defaultValue{defaultValue}, _variadic{variadic} {}


Farg::~Farg() {}

FuncArgs::FuncArgs(lex_token token) : Ast{token} {}

FuncArgs::~FuncArgs() {}

void FuncArgs::append(ast_farg arg)
{
	addChild(arg);
}

FuncDecl::FuncDecl(Token *token, Modifier *mod, Name *name, FuncArgs *args, TypeSpec *retType, Stmt *body, PackageSymbol *package, FuncSymbol *symbol) : ScopedDecl{token, mod, name, body, package, symbol}, _args{args}, _retType{retType}, slots{0}
{
	addChild(args);
}

FuncDecl::FuncDecl(Token *token, FuncSymbol *symbol, FuncArgs *args, TypeSpec *retType, Stmt *body, PackageSymbol *package) : ScopedDecl{token, symbol, body, package}, _args{args}, _retType{retType}, slots{0}
{
    addChild(args);
}

FuncDecl::~FuncDecl() {}

} // namespace hydro
