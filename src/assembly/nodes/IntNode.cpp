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

#include "IntNode.hpp"

namespace hydro
{

IntNode::IntNode(int32_t i) : _value{i} {}

IntNode::~IntNode() {}

void IntNode::build(Chunk *chunk)
{
    chunk->bytes.putInt(_value);
}

} // namespace hydro
