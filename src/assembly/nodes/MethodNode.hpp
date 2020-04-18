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

#ifndef __h3o_MethodNode__
#define __h3o_MethodNode__

#include "FuncNode.hpp"
#include "../../vm/MethodData.hpp"

namespace hydro
{

class MethodNode : public FuncNode
{
private:
	MethodData *_method;

public:
	MethodNode(MethodData *method);
	virtual ~MethodNode();
	virtual void build(Chunk *chunk);
	MethodData *method() const { return _method; }
};

} // namespace hydro

#endif /* __h3o_MethodNode__ */
