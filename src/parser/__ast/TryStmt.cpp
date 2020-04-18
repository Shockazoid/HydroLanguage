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

#include "TryStmt.hpp"

namespace hydro
{

CatchClause::CatchClause(lex_token token, SimpleName *name, TypeSpec *type, Stmt *body, VarSymbol *symbol) : Stmt{token, symbol}, _name{name}, _type{type}, _body{body}
{
	addChild(body);
}

CatchClause::~CatchClause() {}

TryStmt::TryStmt(lex_token token, ast_stmt body) : Stmt{token}, _body{body}
{
	addChild(body);
}

TryStmt::~TryStmt() {}

void TryStmt::append(ast_catch ctch)
{
	addChild(ctch);
}

} // namespace hydro
