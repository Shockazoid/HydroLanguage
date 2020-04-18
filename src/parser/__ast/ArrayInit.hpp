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

#ifndef __h3o_ArrayInit__
#define __h3o_ArrayInit__

#include "Literal.hpp"

namespace hydro
{

class ArrayInit : public Literal
{
public:
	ArrayInit(lex_token token);
	virtual ~ArrayInit();
	void append(ast_expr elementExpr);
};

typedef ArrayInit *ast_array;

} // namespace hydro

#endif /* __h3o_ArrayInit__ */
