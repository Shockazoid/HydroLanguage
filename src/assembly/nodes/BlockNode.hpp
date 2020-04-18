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

#ifndef __h3o_BlockNode__
#define __h3o_BlockNode__

#include <vector>

#include "H3oNode.hpp"
#include "../../vm/VM_Object.hpp"

namespace hydro
{

class BlockNode : public H3oNode
{
protected:
	std::vector<VM_Object *> _slots;
	std::vector<VM_Object *> _upvalues;
	std::vector<H3oNode *> _nodes;

public:
	BlockNode();
	virtual ~BlockNode();
	virtual void addSlot(VM_Object *var);
	virtual void addUpvalue(VM_Object *var);
	virtual void build(Chunk *chunk) override;
	void append(H3oNode *node);
	void appendFront(H3oNode *node);
	int32_t nslots() const { return (int32_t)_slots.size(); }
};

} // namespace hydro

#endif /* __h3o_BlockNode__ */
