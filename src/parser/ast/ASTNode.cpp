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

#include "ASTNode.hpp"

namespace hydro
{

ASTNode::ASTNode(Token *token)
{
    m_parent = nullptr;
    this->token = token;
}

ASTNode::~ASTNode()
{
    for(ASTNode *child : m_children)
        delete child;
}

void ASTNode::accept(ASTVisitor *visitor)
{
    if(visitor->visit(this))
        acceptChildren(visitor);
}

void ASTNode::acceptChildren(ASTVisitor *visitor)
{
    // visit children
    for(ASTNode *child : m_children)
        child->accept(visitor);
}

} // namespace hydro
