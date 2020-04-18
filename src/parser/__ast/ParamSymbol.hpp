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

#ifndef __h3o_ParamSymbol__
#define __h3o_ParamSymbol__

#include "Symbol.hpp"
#include "TypeSpec.hpp"

namespace hydro
{

class ParamSymbol : public Symbol
{
private:
    TypeSpec *_type;
    
public:
    ParamSymbol(Modifier *mod, Name *name, TypeSpec *type) : Symbol{mod, name}, _type{type} {}
    virtual ~ParamSymbol() {}
};

} // namespace hydro

#endif /* __h3o_ParamSymbol__ */
