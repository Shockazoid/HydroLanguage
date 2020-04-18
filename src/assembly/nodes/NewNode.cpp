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

#include "NewNode.hpp"

namespace hydro
{

NewNode::NewNode(std::string qualifiedName, uint32_t constIndex, int32_t nargs) : _qname{qualifiedName}, _constIndex{constIndex}, _nargs{nargs} {}

NewNode::~NewNode() {}

void NewNode::build(Chunk *chunk)
{
    bytearray &bytes = chunk->bytes;
    
	if (hclass c = chunk->runtime->findClass(_qname))
	{
		// replace undefined constant
		chunk->constPool[_constIndex] = c;
	}

	bytes.put(push_instr);
	bytes.putInt(_constIndex);
	bytes.put(new_instr);
	bytes.putInt(_nargs);
}

} // namespace hydro
