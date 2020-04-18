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

#ifndef __h3o_Call__
#define __h3o_Call__

#include "BinaryOp.hpp"
#include "CallArgs.hpp"

namespace hydro
{

class Call : public BinaryOp
{
private:
	ast_cargs _args;

public:
	Call(ast_expr lhs, lex_token token, ast_cargs args);
	virtual ~Call();
	ast_cargs args() const { return _args; }
	unsigned nargs() const { return _args->count(); }
};

typedef Call *ast_call;

} // namespace hydro

#endif /* __h3o_Call__ */
