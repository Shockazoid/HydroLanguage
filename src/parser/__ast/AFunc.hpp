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

#ifndef __h3o_AFunc__
#define __h3o_AFunc__

#include "Expr.hpp"
#include "FuncDecl.hpp"

namespace hydro
{

class AFunc : public Expr
{
private:
	ast_mod _mods;
	ast_fargs _args;
	ast_htype _retType;
	ast_stmt _body;

public:
	AFunc(lex_token token, ast_mod mods, ast_fargs args, ast_htype retType, ast_stmt body = nullptr);
	virtual ~AFunc();
	ast_mod mods() const { return _mods; }
	ast_fargs args() const { return _args; }
	ast_htype returnType() const { return _retType; }
	ast_stmt body() const { return _body; }
};

} // namespace hydro

#endif /* __h3o_AFunc__ */
