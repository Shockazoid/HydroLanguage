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

#ifndef __h3o_ConstructorDecl__
#define __h3o_ConstructorDecl__

#include "FuncDecl.hpp"
#include "ConstructorSymbol.hpp"

namespace hydro
{

class ConstructorDecl : public FuncDecl
{
public:
	ConstructorDecl(lex_token token, ast_mod mods, ast_fargs fargs, ast_htype returnType,
			    ast_stmt block = nullptr, ConstructorSymbol *symbol = nullptr);
	virtual ~ConstructorDecl();
};

typedef ConstructorDecl *ast_construct;

} // namespace hydro

#endif /* __h3o_ConstructorDecl__ */
