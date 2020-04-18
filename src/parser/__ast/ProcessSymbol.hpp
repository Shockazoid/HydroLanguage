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

#ifndef __h3o_ProcessSymbol__
#define __h3o_ProcessSymbol__

#include "Symbol.hpp"

namespace hydro
{

class ProcessSymbol : public Symbol
{
private:
    
public:
    ProcessSymbol(Modifier *mod, Name *name) : Symbol(mod, name) {}
    virtual ~ProcessSymbol() {}
};

} // namespace hydro

#endif /* __h3o_ProcessSymbol__ */
