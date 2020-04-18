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

#ifndef __h3o_ConstructorSymbol__
#define __h3o_ConstructorSymbol__

#include "FuncSymbol.hpp"
#include "PackageSymbol.hpp"
#include "ClassSymbol.hpp"

namespace hydro
{

class ConstructorSymbol : public FuncSymbol
{
private:
    ClassSymbol *_owner;
    
public:
    ConstructorSymbol(Modifier *modifier, Scope *ownScope, ClassSymbol *owner) : FuncSymbol{modifier, nullptr, ownScope}, _owner{owner} {}
	virtual ~ConstructorSymbol() {}
    ClassSymbol *owner() const { return _owner; }
};

} // namespace hydro

#endif /* __h3o_ConstructorSymbol__ */
