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

#ifndef __h3o_EventSymbol__
#define __h3o_EventSymbol__

#include "Symbol.hpp"
#include "PackageSymbol.hpp"

namespace hydro
{

class EventSymbol : public Symbol
{
private:
    PackageSymbol *_package;
    
public:
    EventSymbol(Modifier *mod, Name *name, PackageSymbol *package = nullptr) : Symbol{mod, name}, _package{package} {}
    virtual ~EventSymbol() {}
    PackageSymbol *package() const { return _package; }
};

} // namespace hydro

#endif /* __h3o_EventSymbol__ */
