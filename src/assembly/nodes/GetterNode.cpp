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

#include "GetterNode.hpp"

namespace hydro
{

GetterNode::GetterNode(PropertyNode *owner, VM_Getter *vgetter) : FuncNode{vgetter}, _owner{owner}, _vgetter{vgetter} {}

GetterNode::~GetterNode() {}

void GetterNode::build(Chunk *chunk)
{
    FuncNode::build(chunk);
    if(_args.size() > 0)
        throw std::runtime_error{"Getter defined with invalid arguments."}; // TODO compiler error
    
    _vgetter->property = _owner->property();
    _vgetter->owner = _owner->property()->owner;
    _owner->property()->getter = _vgetter;
}

} // namespace hydro
