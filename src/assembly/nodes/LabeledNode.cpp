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

#include "LabeledNode.hpp"

namespace hydro
{

LabeledNode::LabeledNode(Label *label) : _label{label} {}

LabeledNode::~LabeledNode() {}

void LabeledNode::build(Chunk *chunk)
{
    bytearray &bytes = chunk->bytes;
    
	if (!_label->isWritten())
	{
		// store reference address
		uint32_t addr = bytes.position();

		// write temporary address
		bytes.putInt(-1);

		// add forward reference
		_label->addForwardReference(addr);
	}
	else
	{
		// write label address
		bytes.putInt(_label->addr());
	}
}

} // namespace hydro
