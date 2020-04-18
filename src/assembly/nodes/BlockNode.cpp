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

#include "BlockNode.hpp"

namespace hydro
{

BlockNode::BlockNode() : H3oNode{}, _slots{}, _upvalues{}, _nodes{} {}

BlockNode::~BlockNode()
{
	for (H3oNode *node : _nodes)
		delete node;

	_slots.clear();
	_upvalues.clear();
}

void BlockNode::addSlot(VM_Object *data)
{
	_slots.push_back(data);
}

void BlockNode::addUpvalue(VM_Object *data)
{
	_slots.push_back(data);
	_upvalues.push_back(data);
}

void BlockNode::build(Chunk *chunk)
{
	for (H3oNode *node : _nodes)
	{
		node->build(chunk);
		delete node;
	}

	_nodes.clear();
	buildChildren(chunk);
}

void BlockNode::append(H3oNode *node) { addChild(node); }

void BlockNode::appendFront(H3oNode *node)
{
	_nodes.push_back(node);
}

} // namespace hydro
