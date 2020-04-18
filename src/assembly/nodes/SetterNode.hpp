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

#ifndef __h3o_SetterNode__
#define __h3o_SetterNode__

#include "FuncNode.hpp"
#include "PropertyNode.hpp"
#include "../../vm/SetterData.hpp"
#include "../../vm/NilTypeSpecifier.hpp"

namespace hydro
{

class SetterNode : public FuncNode
{
private:
	SetterData *_vsetter;
    PropertyNode *_owner;
    
public:
	SetterNode(PropertyNode *owner, SetterData *vsetter);
	virtual ~SetterNode();
    virtual void build(Chunk *chunk) override;
	SetterData *vsetter() const { return _vsetter; }
};

} // namespace hydro

#endif /* __h3o_SetterNode__ */
