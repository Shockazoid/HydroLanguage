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

#ifndef __h3o_EtypeSymbol__
#define __h3o_EtypeSymbol__

#include "Symbol.hpp"

namespace hydro
{

class EtypeSymbol : public Symbol
{
public:
    EtypeSymbol(Modifier *mod, Name *name) : Symbol{mod, name, nullptr} {}
    virtual ~EtypeSymbol() {}
};

} // namespace hydro

#endif /* __h3o_EtypeSymbol__ */
