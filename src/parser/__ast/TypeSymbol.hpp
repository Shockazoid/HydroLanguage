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

#ifndef __h3o_TypeSymbol__
#define __h3o_TypeSymbol__

#include "Symbol.hpp"

namespace hydro
{

class TypeSymbol : public Symbol
{
public:
    TypeSymbol(Name *name, Scope *ownScope = nullptr) : Symbol{nullptr, name, ownScope} {}
	//TypeSymbol(std::string name, Scope *ownScope = nullptr) : Symbol{nullptr, name, ownScope} {}
	virtual ~TypeSymbol() {}
};

} // namespace hydro

#endif /* __h3o_TypeSymbol__ */
