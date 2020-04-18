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

#ifndef __h3o_SetterDecl__
#define __h3o_SetterDecl__

#include "FuncDecl.hpp"
#include "Block.hpp"

namespace hydro
{

class SetterDecl final : public FuncDecl
{
public:
	SetterDecl(lex_token token, Modifier *mods, FuncArgs *args = nullptr, Block *body = nullptr);
	virtual ~SetterDecl();
};

typedef SetterDecl *ast_setter;

} // namespace hydro

#endif /* __h3o_SetterDecl__ */
