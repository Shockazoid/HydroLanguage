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

#ifndef __h3o_LibrarySymbol__
#define __h3o_LibrarySymbol__

#include "Symbol.hpp"

namespace hydro
{

class LibrarySymbol : public Symbol
{
private:
    std::string _fullName;
    
public:
    LibrarySymbol(SimpleName *name, Scope *ownScope) : Symbol{nullptr, name, ownScope, nullptr} {}
    virtual ~LibrarySymbol() {}
};

} // namespace hydro


#endif /* __h3o_LibrarySymbol__ */
