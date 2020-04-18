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

#ifndef __h3o_BasicInstrNode__
#define __h3o_BasicInstrNode__

#include <cstdlib>

#include "AssemblyNode.hpp"

namespace hydro
{

class BasicInstrNode : public AssemblyNode
{
private:
    uint8_t _instr;
    uint8_t _noperands;
    int32_t *_operands;

public:
    BasicInstrNode(uint8_t instr);
    BasicInstrNode(uint8_t instr, int32_t opnd1);
    BasicInstrNode(uint8_t instr, int32_t opnd1, int32_t opnd2);
    BasicInstrNode(uint8_t instr, int32_t opnd1, int32_t opnd2, int32_t opnd3);
    virtual ~BasicInstrNode();
    virtual void build(Chunk *chunk) override;
    uint8_t instr() const { return _instr; }
    uint8_t size() const { return _noperands; }
    const int32_t *operands() const { return _operands; }
};

} // namespace hydro

#endif /* __h3o_BasicInstrNode__ */
