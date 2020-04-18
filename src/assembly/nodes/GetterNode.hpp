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

#ifndef __h3o_GetterNode__
#define __h3o_GetterNode__

#include "FuncNode.hpp"
#include "PropertyNode.hpp"
#include "../../vm/VM_Getter.hpp"

namespace hydro
{

class GetterNode : public FuncNode
{
private:
    PropertyNode *_owner;
	VM_Getter *_vgetter;

public:
	GetterNode(PropertyNode *owner, VM_Getter *getter);
	virtual ~GetterNode();
    virtual void build(Chunk *chunk) override;
	VM_Getter *vgetter() const { return _vgetter; }
};

} // namespace hydro

#endif /* __h3o_GetterNode__ */
