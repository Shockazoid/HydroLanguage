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

#include "Block.hpp"

namespace hydro
{

Block::Block(lex_token token) : Stmt{token} {}

Block::~Block() {}

void Block::append(ast_stmt stmt)
{
	addChild(stmt);
}

} // namespace hydro
