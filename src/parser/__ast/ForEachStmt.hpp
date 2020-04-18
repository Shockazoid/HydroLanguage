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

#ifndef __h3o_ForEachStmt__
#define __h3o_ForEachStmt__

#include "Stmt.hpp"
#include "Expr.hpp"
#include "Name.hpp"

namespace hydro
{

class ForEachStmt : public Stmt
{
private:
	ast_name _valueName;
	ast_name _keyName;
	ast_expr _expr;
	ast_stmt _body;

public:
	ForEachStmt(lex_token token, ast_name valueName, ast_expr expr, ast_stmt body);
	ForEachStmt(lex_token token, ast_name keyName, ast_name valueName, ast_expr expr, ast_stmt body);
	virtual ~ForEachStmt();
	ast_name valueName() const { return _valueName; }
	ast_name keyName() const { return _keyName; }
	ast_expr expr() const { return _expr; }
	ast_stmt body() const { return _body; }
};

typedef ForEachStmt *ast_foreach;

} // namespace hydro

#endif /* __h3o_ForEachStmt__ */
