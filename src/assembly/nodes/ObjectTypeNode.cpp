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

#include "ObjectTypeNode.hpp"

namespace hydro
{

ObjectTypeNode::ObjectTypeNode() {}


ObjectTypeNode::~ObjectTypeNode() {}

void ObjectTypeNode::build(Chunk *chunk)
{
    
}

TypeSpecifier *ObjectTypeNode::make()
{
    return new ObjectTypeSpecifier{""};
}

} // namespace hydro
