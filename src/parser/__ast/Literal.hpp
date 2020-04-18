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

#ifndef __h3o_Literal__
#define __h3o_Literal__

#include "Expr.hpp"

namespace hydro
{

class Literal : public Expr
{
public:
	Literal(lex_token literalToken);
	virtual ~Literal();
};

typedef Literal *ast_literal;

} // namespace hydro

#endif /* __h3o_Literal__ */
