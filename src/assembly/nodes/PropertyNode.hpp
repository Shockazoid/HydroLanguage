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

#ifndef __h3o_PropertyNode__
#define __h3o_PropertyNode__

#include "BlockNode.hpp"
#include "InstrGroupNode.hpp"
#include "../../vm/VM_Property.hpp"

namespace hydro
{

class PropertyNode : public BlockNode
{
private:
	VM_Property *_property;

public:
    InstrGroupNode *initValue{nullptr};
	PropertyNode(VM_Property *property);
	virtual ~PropertyNode();
	virtual void build(Chunk *chunk);
	VM_Property *property() const { return _property; }
};

} // namespace hydro

#endif /* __h3o_PropertyNode__ */
