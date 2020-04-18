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

#ifndef __h3o_WhileStmt__
#define __h3o_WhileStmt__

#include "Stmt.hpp"
#include "Expr.hpp"

namespace hydro
{

class WhileStmt : public Stmt
{
private:
	ast_expr _cond;
	ast_stmt _body;

public:
	WhileStmt(lex_token token, Expr *cond, Stmt *body);
	virtual ~WhileStmt();
	ast_expr cond() const { return _cond; }
	ast_stmt body() const { return _body; }
};

typedef WhileStmt *ast_while;

} // namespace hydro

#endif /* __h3o_WhileStmt__ */
