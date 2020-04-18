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

#ifndef __h3o_ASTNode__
#define __h3o_ASTNode__

#include <list>

#include "../Token.hpp"
#include "ASTVisitor.hpp"

namespace hydro
{

class ASTNode
{
private:
    ASTNode *m_parent;
    std::list<ASTNode *> m_children;
    
protected:
    ASTNode(Token *token = nullptr);
    
public:
    Token *token;
    virtual ~ASTNode();
    virtual void accept(ASTVisitor *visitor);
    virtual void acceptChildren(ASTVisitor *visitor);
    ASTNode *owner() const { return m_parent; }
    ASTNode *parent() const { return m_parent; }
};

} // namespace hydro

#endif /* __h3o_ASTNode__ */
