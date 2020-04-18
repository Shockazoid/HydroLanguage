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

#ifndef __h3o_TernaryExpression__
#define __h3o_TernaryExpression__

#include "Expression.hpp"

namespace hydro
{

class TernaryExpression : public Expression
{
public:
    Expression *aExpr;
    Expression *bExpr;
    Expression *cExpr;
    TernaryExpression();
    virtual ~TernaryExpression();
};

} // namespace hydro

#endif /* __h3o_TernaryExpression__ */
