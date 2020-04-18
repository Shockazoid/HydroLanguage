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

#ifndef __h3o_GetterDecl__
#define __h3o_GetterDecl__

#include "FuncDecl.hpp"
#include "Block.hpp"

namespace hydro
{

class GetterDecl : public FuncDecl
{
public:
	GetterDecl(lex_token token, Modifier *mods, FuncArgs *args = nullptr, Block *body = nullptr);
	virtual ~GetterDecl();
};

typedef GetterDecl *ast_getter;

} // namespace hydro

#endif /* __h3o_GetterDecl__ */
