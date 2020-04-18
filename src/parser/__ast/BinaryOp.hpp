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

#ifndef __h3o_BinaryOp__
#define __h3o_BinaryOp__

#include "Expr.hpp"

namespace hydro
{

class BinaryOp : public Expr
{
private:
	ast_expr _lhs;
	ast_expr _rhs;

public:
	BinaryOp(ast_expr lhs, lex_token token, ast_expr rhs);
	virtual ~BinaryOp();
	ast_expr lhs() const { return _lhs; }
	ast_expr rhs() const { return _rhs; }
};

typedef BinaryOp *ast_binary;

} // namespace hydro

#endif /* __h3o_BinaryOp__ */
