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

#ifndef __h3o_AstSymbol__
#define __h3o_AstSymbol__

#include "Name.hpp"
#include "TypeSpec.hpp"

namespace hydro
{
    
class AstSymbol
{
private:
    Modifier *_modifier;
    Name *_name;
    TypeSpec *_type;
    
public:
    AstSymbol(Name *name = nullptr, TypeSpec *type = nullptr);
    AstSymbol(Modifier *modifier, Name *name = nullptr, TypeSpec *type = nullptr);
    virtual ~AstSymbol();
    Modifier *modifier() const { return _modifier; }
    Name *name() const { return _name; }
    TypeSpec *type() const { return _type; }
};

} // namespace hydro

#endif /* __h3o_AstSymbol__ */
