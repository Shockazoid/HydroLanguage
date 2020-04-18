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

#ifndef __h3o_TernaryOp__
#define __h3o_TernaryOp__

#include "Expr.hpp"

namespace hydro
{

class TernaryOp : public Expr
{
private:
	ast_expr _expr1;
	ast_expr _expr2;
	ast_expr _expr3;
	lex_token _token2;

public:
	TernaryOp(ast_expr expr1, lex_token token1, ast_expr expr2, lex_token token2, ast_expr expr3);
	virtual ~TernaryOp();
	ast_expr expr1() const { return _expr1; }
	ast_expr expr2() const { return _expr2; }
	ast_expr expr3() const { return _expr3; }
	lex_token token2() const { return _token2; }
};

typedef TernaryOp *ast_ternary;

} // namespace hydro

#endif /* __h3o_TernaryOp__ */
