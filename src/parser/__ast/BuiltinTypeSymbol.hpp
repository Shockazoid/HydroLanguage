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

#ifndef __h3o_BuiltinTypeSymbol__
#define __h3o_BuiltinTypeSymbol__

#include "TypeSymbol.hpp"

namespace hydro
{

class BuiltinTypeSymbol : public TypeSymbol
{
public:
	BuiltinTypeSymbol(Name *name) : TypeSymbol{name} {}
	virtual ~BuiltinTypeSymbol() {}
};

} // namespace hydro

#endif /* __h3o_BuiltinTypeSymbol__ */
