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

#ifndef __h3o_EventDecl__
#define __h3o_EventDecl__

#include "TypeDecl.hpp"
#include "Block.hpp"
#include "EventSymbol.hpp"

namespace hydro
{

class EventDecl : public TypeDecl
{
public:
	EventDecl(lex_token token, Modifier *mods, Name *name, Block *body, EventSymbol *symbol, PackageSymbol *package = nullptr);
	virtual ~EventDecl();
};

} // namespace hydro

#endif /* __h3o_EventDecl__ */
