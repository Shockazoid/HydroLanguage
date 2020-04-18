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

#ifndef __h3o_DoStmt_hpp
#define __h3o_DoStmt_hpp

#include "Stmt.hpp"
#include "Expr.hpp"

namespace hydro
{

class DoStmt : public Stmt
{
private:
	ast_stmt _body;
	ast_expr _cond;

public:
	DoStmt(lex_token token, ast_stmt body, ast_expr cond);
	virtual ~DoStmt();
	ast_stmt body() const { return _body; }
	ast_expr cond() const { return _cond; }
};

typedef DoStmt *ast_do;

} // namespace hydro

#endif /* __h3o_DoStmt_hpp */
