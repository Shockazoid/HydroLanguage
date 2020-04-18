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

#ifndef __h3o_SwitchStmt__
#define __h3o_SwitchStmt__

#include "Stmt.hpp"
#include "Expr.hpp"

namespace hydro
{

class CaseClause : public Stmt
{
private:
	ast_expr _expr;

public:
	CaseClause(lex_token token, ast_expr expr);
	virtual ~CaseClause();
	ast_expr expr() const { return _expr; }
};

typedef CaseClause *ast_case;

class DefaultClause : public Stmt
{
public:
	DefaultClause(lex_token);
	virtual ~DefaultClause();
};

typedef DefaultClause *ast_default;

class SwitchStmt : public Stmt
{
private:
	ast_expr _expr;
	ast_stmt _body;

public:
	SwitchStmt(lex_token token, ast_expr expr, ast_stmt body);
	virtual ~SwitchStmt();
	ast_expr expr() const { return _expr; }
	ast_stmt body() const { return _body; }
};

typedef SwitchStmt *ast_switch;

} // namespace hydro

#endif /* __h3o_SwitchStmt__ */
