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

#include "AstSymbol.hpp"

namespace hydro
{

AstSymbol::AstSymbol(Modifier *modifier, Name *name, TypeSpec *type) : _modifier{modifier}, _name{name}, _type{type} {}

AstSymbol::AstSymbol(Name *name, TypeSpec *type) : _modifier{nullptr}, _name{name}, _type{type} {}

AstSymbol::~AstSymbol() {}

} // namespace hydro
