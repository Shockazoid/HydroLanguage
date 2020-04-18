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

#include "InstrGroupNode.hpp"

namespace hydro
{

InstrGroupNode::InstrGroupNode(InstrGroup *group) : BlockNode{}, _group{group} {}

InstrGroupNode::~InstrGroupNode() {}

void InstrGroupNode::build(Chunk *chunk)
{
    bytearray &bytes = chunk->bytes;
    
    // set chunk
    _group->chunk = chunk;
    
	// get start bounds
	_group->startAddr = bytes.position();

	// build instructions
	buildChildren(chunk);

	// tell the vm to return the top of the stack if returning a value
	if (returnsValue)
		bytes.put(ret_instr);

	// get end bounds
	_group->endAddr = bytes.position();
}

} // namespace hydro
