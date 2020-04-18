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

#include "PropertyNode.hpp"

namespace hydro
{

PropertyNode::PropertyNode(VM_Property *property) : _property{property} {}

PropertyNode::~PropertyNode() {}

void PropertyNode::build(Chunk *chunk)
{
    buildChildren(chunk);
    
    if(initValue)
    {
        // emit value
        initValue->build(chunk);
        _property->initValue = initValue->group();
    }
}

} // namespace hydro
