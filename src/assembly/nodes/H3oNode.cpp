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

#include "H3oNode.hpp"

namespace hydro
{

H3oNode::H3oNode() {}

H3oNode::~H3oNode() {}

void H3oNode::buildChildren(Chunk *chunk)
{
    Ast *child = firstChild();

    while (child)
    {
        if (H3oNode *node = child->tryCast<H3oNode>())
            node->build(chunk);
        child = child->next();
    }
}

void H3oNode::build(Chunk *chunk)
{
    buildChildren(chunk);
}

} // namespace hydro
