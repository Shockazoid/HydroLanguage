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

#ifndef __h3o_ExpressionStatement__
#define __h3o_ExpressionStatement__

#include "Statement.hpp"
#include "Expression.hpp"

namespace hydro
{

class ExpressionStatement : public Statement
{
public:
    Expression *expr;
    ExpressionStatement() : Statement{}, expr{nullptr} {}
    virtual ~ExpressionStatement() {}
};

} // namespace hydro

#endif /* __h3o_ExpressionStatement__ */
