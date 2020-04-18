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

#include "AssemblyNode.hpp"

namespace hydro
{

AssemblyNode::AssemblyNode(Token *token) : m_token{token}, m_parent{nullptr} {}

AssemblyNode::~AssemblyNode()
{
    for(AssemblyNode *child : m_children)
        delete child;
}

void AssemblyNode::prependChild(AssemblyNode *child)
{
   if(child == this)
        return;
    
    if(child->m_parent)
        child->m_parent->removeChild(child);
    
    child->m_parent = this;
    m_children.push_front(child);
}

void AssemblyNode::appendChild(AssemblyNode *child)
{
    if(child == this)
        return;
    
    if(child->m_parent)
        child->m_parent->removeChild(child);
    
    child->m_parent = this;
    m_children.push_back(child);
}

void AssemblyNode::removeChild(AssemblyNode *child)
{
    if(child->m_parent == this)
    {
        child->m_parent = nullptr;
        m_children.remove(child);
    }
}

void AssemblyNode::build(Chunk *chunk)
{
    for(AssemblyNode *child : m_children)
        child->build(chunk);
}

} // namespace hydro
