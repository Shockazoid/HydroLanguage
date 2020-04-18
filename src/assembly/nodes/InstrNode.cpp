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

#include "InstrNode.hpp"
#include "../../vm/opcodes.hpp"

namespace hydro
{

InstrNode::InstrNode(uint8_t instr) : _instr{instr}, _noperands{0}, _operands{nullptr} {}

InstrNode::InstrNode(uint8_t instr, int32_t opnd1) : _instr{instr}, _noperands{1}, _operands{new int32_t[1]{opnd1}} {}

InstrNode::InstrNode(uint8_t instr, int32_t opnd1, int32_t opnd2) : _instr{instr}, _noperands{2}, _operands{new int32_t[2]{opnd1, opnd2}} {}

InstrNode::InstrNode(uint8_t instr, int32_t opnd1, int32_t opnd2, int32_t opnd3) : _instr{instr}, _noperands{3}, _operands{new int32_t[3]{opnd1, opnd2, opnd3}} {}

InstrNode::~InstrNode()
{
	delete[] _operands;
}

void InstrNode::build(Chunk *chunk)
{
    bytearray &bytes = chunk->bytes;
	bytes.put(_instr);
	uint8_t n = 0;

	while (n < _noperands)
		bytes.putInt(_operands[n++]);
}

} // namespace hydro
