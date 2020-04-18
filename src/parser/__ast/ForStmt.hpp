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

#ifndef __h3o_ForStmt__
#define __h3o_ForStmt__

#include "Stmt.hpp"
#include "Expr.hpp"

namespace hydro
{

class ForStmt : public Stmt
{
private:
	ast_expr _init; // initializer
	ast_expr _cond; // condition
	ast_expr _it;   // iterator
	ast_stmt _body;

public:
	ForStmt(lex_token token, ast_expr init, ast_expr cond, ast_expr it, ast_stmt body);
	virtual ~ForStmt();
	ast_expr init() const { return _init; }
	ast_expr cond() const { return _cond; }
	ast_expr it() const { return _it; }
	ast_stmt body() const { return _body; }
};

typedef ForStmt *ast_for;

} // namespace hydro

#endif /* ForStmt_hpp */
