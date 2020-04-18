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

#ifndef __h3o_EtypeDecl__
#define __h3o_EtypeDecl__

#include "Decl.hpp"

namespace hydro
{
class EtypeDecl : public Decl
{
public:
	EtypeDecl(lex_token token, Modifier *mods, Name *name);
	virtual ~EtypeDecl();
};

} // namespace hydro

#endif /* __h3o_EtypeDecl__ */
