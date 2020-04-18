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

#include "CatchNode.hpp"

namespace hydro
{

CatchNode::CatchNode(int32_t slot, TypeNode *type, Label *from, Label *to, Label *jump) : _slot{slot}, _type{type}, _errorHandler{nullptr}, _from{from}, _to{to}, _jump{jump} {}

CatchNode::~CatchNode() {}

void CatchNode::build(Chunk *chunk)
{
    // create handler
    _errorHandler = new ErrorHandler{};
    _errorHandler->type = new NilTypeSpecifier{};
    _errorHandler->errorSlot = _slot;
    
    // save error handler start address
    _errorHandler->startAddr = chunk->bytes.position();
    _errorHandler->fromAddr = _from->addr();
    _errorHandler->toAddr = _to->addr();
    
    // emit error handling bytes
    buildChildren(chunk);
    
    // emit jump
    chunk->bytes.put(jmp_instr);
    
    // add forward reference
    _jump->addForwardReference(chunk->bytes.position());
    
    // emit temporary address
    chunk->bytes.putInt(-1);
}

} // namespace hydro
