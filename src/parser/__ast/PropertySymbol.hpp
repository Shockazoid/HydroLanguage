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

#ifndef __h3o_PropertySymbol__
#define __h3o_PropertySymbol__

#include "TypeSpec.hpp"
#include "ClassSymbol.hpp"

namespace hydro
{

class PropertySymbol : public Symbol
{
private:
    TypeSpec *_type;
    ClassSymbol *_owner;
    
public:
    PropertySymbol(Modifier *mod, Name *name, TypeSpec *type, ClassSymbol *owner, Scope *ownScope = nullptr) : Symbol{mod, name, ownScope}, _type{type}, _owner{owner} {}
    virtual ~PropertySymbol() {}
    TypeSpec *type() const { return _type; }
    ClassSymbol *owner() const { return _owner; }
};

} // namespace hydro

#endif /* __h3o_PropertySymbol__ */
