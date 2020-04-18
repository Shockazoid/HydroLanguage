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

#ifndef __h3o_TryStmt__
#define __h3o_TryStmt__

#include "Stmt.hpp"
#include "TypeSpec.hpp"
#include "SimpleName.hpp"
#include "VarSymbol.hpp"

namespace hydro
{

class CatchClause : public Stmt
{
private:
    SimpleName *_name;
    TypeSpec *_type;
	ast_stmt _body;

public:
	CatchClause(lex_token token, SimpleName *name, TypeSpec *type, ast_stmt body, VarSymbol *symbol);
	virtual ~CatchClause();
    SimpleName *name() const { return _name; }
    TypeSpec *type() const { return _type; }
	ast_stmt body() const { return _body; }
};

typedef CatchClause *ast_catch;

class TryStmt : public Stmt
{
private:
	ast_stmt _body;

public:
	TryStmt(lex_token token, Stmt *stmt);
	virtual ~TryStmt();
	void append(ast_catch ctch);
	ast_stmt body() const { return _body; }
};

typedef TryStmt *ast_try;

} // namespace hydro

#endif /* __h3o_TryStmt__ */
