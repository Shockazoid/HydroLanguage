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

#ifndef __h3o_NewNode__
#define __h3o_NewNode__

#include "H3oNode.hpp"
#include "../../system/HClass.hpp"
#include "../../vm/Chunk.hpp"
#include "../../vm/Runtime.hpp"
#include "../../vm/opcodes.hpp"

namespace hydro
{

class NewNode : public H3oNode
{
private:
	std::string _qname;
	uint32_t _constIndex;
	int32_t _nargs;

public:
	NewNode(std::string qualifiedName, uint32_t constIndex, int32_t nargs);
	virtual ~NewNode();
	virtual void build(Chunk *chunk) override;
};

} // namespace hydro

#endif /* __h3o_NewNode__ */
