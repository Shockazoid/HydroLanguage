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

#ifndef __h3o_InstrNode__
#define __h3o_InstrNode__

#include <cstdlib>

#include "H3oNode.hpp"

namespace hydro
{

class InstrNode : public H3oNode
{
private:
	uint8_t _instr;
	uint8_t _noperands;
	int32_t *_operands;

public:
	InstrNode(uint8_t instr);
	InstrNode(uint8_t instr, int32_t opnd1);
	InstrNode(uint8_t instr, int32_t opnd1, int32_t opnd2);
	InstrNode(uint8_t instr, int32_t opnd1, int32_t opnd2, int32_t opnd3);
	virtual ~InstrNode();
	virtual void build(Chunk *chunk) override;
	uint8_t instr() const { return _instr; }
	uint8_t size() const { return _noperands; }
	const int32_t *operands() const { return _operands; }
};

} // namespace hydro

#endif /* __h3o_InstrNode__ */
