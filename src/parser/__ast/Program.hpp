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

#ifndef __h3o_Program__
#define __h3o_Program__

#include "Stmt.hpp"

namespace hydro
{

class Program : public Ast
{
public:
    int32_t slots;
	Program();
	virtual ~Program();
	void append(ast_stmt stmt);
};

typedef Program *ast_program;

} // namespace hydro

#endif /* __h3o_Program__ */
