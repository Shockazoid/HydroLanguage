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

#include "ActionNode.hpp"
#include "../../system/HEventContext.hpp"
#include "../../vm/Runtime.hpp"

namespace hydro
{

ActionNode::ActionNode(VM_Action *vact, std::string contextName) : FuncNode{vact}, _vact{vact}, _contextName{contextName} {}

ActionNode::~ActionNode() {}

void ActionNode::build(Chunk *chunk)
{
    FuncNode::build(chunk);
}

} // namespace hydro
