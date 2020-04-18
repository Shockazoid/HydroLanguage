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

#ifndef __h3o_StringTemplate__
#define __h3o_StringTemplate__

#include "Literal.hpp"

namespace hydro
{

class StringTemplate : public Literal
{
public:
	StringTemplate(lex_token token);
	virtual ~StringTemplate();
	void append(ast_expr expr);
};

typedef StringTemplate *ast_template;

} // namespace hydro

#endif /* __h3o_StringTemplate__ */
