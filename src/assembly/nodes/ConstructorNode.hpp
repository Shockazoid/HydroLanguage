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

#ifndef __h3o_ConstructorNode__
#define __h3o_ConstructorNode__

#include "FuncNode.hpp"
#include "../../vm/ConstructorData.hpp"

namespace hydro
{

class ConstructorNode : public FuncNode
{
private:
	ConstructorData *_constructor;

public:
	ConstructorNode(ConstructorData *constructor);
	virtual ~ConstructorNode();
	virtual void build(Chunk *chunk) override;
	ConstructorData *constructor() const { return _constructor; }
};

} // namespace hydro

#endif /* __h3o_ConstructorNode__ */
