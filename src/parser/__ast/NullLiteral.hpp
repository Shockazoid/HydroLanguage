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

#ifndef __h3o_NullLiteral__
#define __h3o_NullLiteral__

#include "Literal.hpp"

namespace hydro
{

class NullLiteral : public Literal
{
public:
    NullLiteral(Token *token);
    virtual ~NullLiteral();
};

} // namespace hydro

#endif /* __h3o_NullLiteral__ */
