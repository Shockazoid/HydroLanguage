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

#ifndef __h3o_UnaryExpression__
#define __h3o_UnaryExpression__

#include "Expression.hpp"

namespace hydro
{

class UnaryExpression : public Expression
{
public:
    Expression *expr{nullptr};
    UnaryExpression() : Expression{} {}
    virtual ~UnaryExpression() {}
};

} // namespace hydro

#endif /* __h3o_UnaryExpression__ */
