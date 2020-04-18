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

#ifndef __h3o_ThrowStmt__
#define __h3o_ThrowStmt__

#include "Stmt.hpp"
#include "Expr.hpp"

namespace hydro
{

class ThrowStmt : public Stmt
{
private:
	ast_expr _expr;

public:
	ThrowStmt(lex_token token, ast_expr expr);
	virtual ~ThrowStmt();
	ast_expr expr() const { return _expr; }
};

typedef ThrowStmt *ast_throw;

} // namespace hydro

#endif /* __h3o_ThrowStmt__ */
