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

#ifndef __h3o_Block__
#define __h3o_Block__

#include "Stmt.hpp"

namespace hydro
{

class Block : public Stmt
{
public:
	Block(lex_token token = nullptr);
	virtual ~Block();
	void append(ast_stmt stmt);
};

typedef Block *ast_block;

} // namespace hydro

#endif /* __h3o_Block__ */
