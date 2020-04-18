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

#include "LabelNode.hpp"

namespace hydro
{

LabelNode::LabelNode(std::string value) : AssemblyNode{}, _value{value}, _addr{-1}, _defined{false}, _written{false}, _fwdRefs{} {}

LabelNode::~LabelNode() { _fwdRefs.clear(); }

void LabelNode::resolveForwardReferences(bytearray &bytes)
{
    for (uint32_t refAddr : _fwdRefs)
        bytes.putInt(_addr, refAddr); // resolver); // back to original position
}

void LabelNode::build(Chunk *chunk)
{
    bytearray &bytes = chunk->bytes;
    
    if (_written)
        return;

    _written = true;
    _addr = bytes.position();

    // resolve forward references
    for (uint32_t r : _fwdRefs)
    {
        bytes.seek(r);
        bytes.putInt(_addr); // resolve
    }

    // restore
    bytes.seek(_addr);
}

} // namespace hydro
