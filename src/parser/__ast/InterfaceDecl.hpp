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

#ifndef __h3o_InterfaceDecl__
#define __h3o_InterfaceDecl__

#include "Decl.hpp"

namespace hydro
{

class InterfaceDecl : public Decl
{
public:
	InterfaceDecl(lex_token token, Modifier *mods, Name *name);
	virtual ~InterfaceDecl();
};
} // namespace hydro

#endif /* __h3o_InterfaceDecl__ */
