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

#ifndef __h3o_ExprStmt__
#define __h3o_ExprStmt__

#include "Expr.hpp"
#include "Stmt.hpp"

namespace hydro
{

class ExprStmt : public Stmt
{
private:
	ast_expr _expr;

public:
	ExprStmt(ast_expr expr);
	virtual ~ExprStmt();
	ast_expr expr() const { return _expr; }
};

typedef ExprStmt *ast_exprstmt;

} // namespace hydro

#endif /* __h3o_ExprStmt__ */
