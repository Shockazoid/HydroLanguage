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

#ifndef __h3o_BinaryExpression__
#define __h3o_BinaryExpression__

#include "Expression.hpp"

namespace hydro
{

class BinaryExpression : public Expression
{
public:
    Expression *lhs{nullptr};
    Expression *rhs{nullptr};
    BinaryExpression(Token *token) : Expression{token} {}
    virtual ~BinaryExpression() {}
};

} // namespace hydro

#endif /* __h3o_BinaryExpression__ */
