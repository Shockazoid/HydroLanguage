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

#ifndef __h3o_TupleInit__
#define __h3o_TupleInit__

#include "Literal.hpp"

namespace hydro
{

class TupleInit : public Literal
{
public:
	TupleInit(lex_token token);
	virtual ~TupleInit();
	void append(ast_expr expr);
};

typedef TupleInit *ast_tuple;

} // namespace hydro

#endif /* __h3o_TupleInit__ */
