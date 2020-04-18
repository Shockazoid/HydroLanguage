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

#ifndef __h3o_EnumDecl__
#define __h3o_EnumDecl__

#include "Decl.hpp"

namespace hydro
{

class EnumDecl : public Decl
{
public:
	EnumDecl(lex_token token, Modifier *mods, Name *name);
	virtual ~EnumDecl();
};

} // namespace hydro

#endif /* __h3o_EnumDecl__ */
