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

#ifndef __h3o_Stmt__
#define __h3o_Stmt__

#include "Ast.hpp"

namespace hydro
{

class Stmt : public Ast
{
public:
	Stmt(lex_token token = nullptr, Symbol *symbol = nullptr);
	virtual ~Stmt();
};

typedef Stmt *ast_stmt;

} // namespace hydro

#endif /* __h3o_Stmt__ */
