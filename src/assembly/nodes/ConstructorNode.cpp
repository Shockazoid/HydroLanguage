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

#include "ConstructorNode.hpp"

namespace hydro
{

ConstructorNode::ConstructorNode(VM_Constructor *constructor) : FuncNode{constructor} {}

ConstructorNode::~ConstructorNode() {}

void ConstructorNode::build(Chunk *chunk)
{
	FuncNode::build(chunk);
}

} // namespace hydro
