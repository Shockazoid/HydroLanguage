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

#ifndef __h3o_ActSymbol__
#define __h3o_ActSymbol__

#include "TypeSymbol.hpp"
#include "Name.hpp"

namespace hydro
{

class ActSymbol : public Symbol
{
private:
    Name *_context;
    PackageSymbol *_package;
    
public:
    ActSymbol(Modifier *modifier, Name *name, Name *context, Scope *ownScope, PackageSymbol *package = nullptr) : Symbol{modifier, name, ownScope, package}, _context{context}, _package{package} {}
    virtual ~ActSymbol() {}
    PackageSymbol *package() const { return _package; }
};

} // namespace hydro

#endif /* __h3o_ActSymbol__ */
