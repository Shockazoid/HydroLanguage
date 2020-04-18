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

#ifndef __h3o_InstrGroupNode__
#define __h3o_InstrGroupNode__

#include "BlockNode.hpp"
#include "../../vm/InstrGroup.hpp"
#include "../../vm/opcodes.hpp"

namespace hydro
{

class InstrGroupNode : public BlockNode
{
protected:
	InstrGroup *_group;
	std::vector<VM_Object *> _slots;
	std::vector<VM_Object *> _upvalues;

public:
	bool returnsValue{false};
	InstrGroupNode(InstrGroup *group);
	virtual ~InstrGroupNode();
	virtual void build(Chunk *chunk) override;
	InstrGroup *group() const { return _group; }
};

} // namespace hydro

#endif /* __h3o_InstrGroupNode__ */
