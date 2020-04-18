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

#ifndef __h3o_ContinueStmt__
#define __h3o_ContinueStmt__

#include "Stmt.hpp"
#include "Expr.hpp"

namespace hydro
{

class ContinueStmt : public Stmt
{
private:
	ast_expr _expr;

public:
	ContinueStmt(lex_token token, ast_expr expr = nullptr);
	virtual ~ContinueStmt();
	ast_expr expr() const { return _expr; }
};

typedef ContinueStmt *ast_continue;

} // namespace hydro

#endif /* __h3o_ContinueStmt__ */
