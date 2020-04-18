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

#ifndef __h3o_PackageSymbol__
#define __h3o_PackageSymbol__

#include <list>

#include "Symbol.hpp"

namespace hydro
{

class PackageSymbol : public Symbol
{
private:
    PackageSymbol *_parentPackage;
    std::string _fullName;
    
public:
    PackageSymbol(Name *name, Scope *ownScope, PackageSymbol *parent = nullptr);
    virtual ~PackageSymbol();
    PackageSymbol *parentPackage() const { return _parentPackage; }
    std::string fullPackageName() const { return _fullName; }
};

} // namespace hydro

#endif /* __h3o_PackageSymbol__ */
