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

#ifndef __h3o_ReturnStmt__
#define __h3o_ReturnStmt__

#include "Stmt.hpp"
#include "Expr.hpp"

namespace hydro
{

class ReturnStmt : public Stmt
{
private:
	Expr *_expr;

public:
	ReturnStmt(lex_token token, Expr *expr = nullptr);
	virtual ~ReturnStmt();
	Expr *expr() const { return _expr; }
};

typedef ReturnStmt *ast_return;

} // namespace hydro

#endif /* __h3o_ReturnStmt__ */
