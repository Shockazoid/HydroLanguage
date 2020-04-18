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

#include "AFunc.hpp"

namespace hydro
{

AFunc::AFunc(lex_token token, ast_mod mods, ast_fargs args, ast_htype retType, ast_stmt body) : Expr{token}, _mods{mods}, _args{args}, _retType{retType}, _body{body}
{
	addChild(args);
}

AFunc::~AFunc() {}

} // namespace hydro
