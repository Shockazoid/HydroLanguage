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

#include "SimpleName.hpp"

namespace hydro
{

SimpleName::SimpleName(Token *token, bool identifier) : Name{token}, _value{token->value()}, isIdentifier{identifier} {}

SimpleName::SimpleName(std::string value, bool identifier) : Name{}, _value{value}, isIdentifier{identifier} {}

SimpleName::~SimpleName() {}

} // namespace hydro
