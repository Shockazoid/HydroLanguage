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

#include "PackageSymbol.hpp"


namespace hydro
{

PackageSymbol::PackageSymbol(Name *name, Scope *ownScope, PackageSymbol *parent) : Symbol{nullptr, name, ownScope, parent}, _parentPackage{parent}
{
    std::list<std::string> tokens;
    _fullName = "";
    PackageSymbol *pkg = this;
    
    while(pkg)
    {
        tokens.push_front(pkg->name()->value());
        pkg = pkg->_parentPackage;
    }
    
    for(std::string name : tokens)
    {
        if(!_fullName.empty())
            _fullName += "." + name;
        else
            _fullName = name;
    }
}

PackageSymbol::~PackageSymbol() {}

} // namespace hydro
