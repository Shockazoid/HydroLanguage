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

#ifndef __h3o_BreakStmt__
#define __h3o_BreakStmt__

#include "Stmt.hpp"
#include "Expr.hpp"

namespace hydro
{

class BreakStmt : public Stmt
{
private:
	ast_expr _expr;

public:
	BreakStmt(lex_token token, ast_expr expr = nullptr);
	virtual ~BreakStmt();
	ast_expr expr() const { return _expr; }
};

typedef BreakStmt *ast_break;

} // namespace hydro

#endif /* __h3o_BreakStmt__ */
