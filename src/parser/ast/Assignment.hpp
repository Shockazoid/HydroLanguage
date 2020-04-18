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

#ifndef __h3o_Assignment__
#define __h3o_Assignment__

#include "BinaryExpression.hpp"

namespace hydro
{

class Assignment : public BinaryExpression
{
public:
    Assignment(Token *token) : BinaryExpression{token} {}
    virtual ~Assignment() {}
};

} // namespace hydro

#endif /* __h3o_Assignment__ */
