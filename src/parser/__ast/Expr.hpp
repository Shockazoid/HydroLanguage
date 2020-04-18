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

#ifndef __h3o_Expr__
#define __h3o_Expr__

#include "Ast.hpp"

namespace hydro
{

class Expr : public Ast
{
public:
	Expr(lex_token token = nullptr);
	virtual ~Expr();
};

typedef Expr *ast_expr;

} // namespace hydro

#endif /* __h3o_Expr__ */
