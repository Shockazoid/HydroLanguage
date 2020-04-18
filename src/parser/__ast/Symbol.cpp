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

#include "Symbol.hpp"
#include "Scope.hpp"

namespace hydro
{

Symbol::Symbol(Modifier *modifier, Name *name, Scope *ownScope, Symbol *parent) : _name{name}, _ownScope{ownScope}, _modifier{modifier}, _parent{parent}
{
    if(ownScope)
        ownScope->_symbol = this;
}

Symbol::~Symbol() {}

bool Symbol::hasMod(std::string modifier) const
{
    return _modifier->hasMod(modifier);
}

int32_t Symbol::index()
{
    return _ownScope ? _ownScope->indexOf(this) : -1;
}

} // namespace hydro
