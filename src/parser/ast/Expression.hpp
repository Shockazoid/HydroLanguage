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

#ifndef __h3o_Expression__
#define __h3o_Expression__

#include "ASTNode.hpp"

namespace hydro
{

class Expression : public ASTNode
{
protected:
    Expression(Token *token = nullptr) : ASTNode{token} {}

public:
    virtual ~Expression() {}
    virtual void accept(ASTVisitor *visitor) override;
};

} // namespace hydro

#endif /* __h3o_Expression__ */
