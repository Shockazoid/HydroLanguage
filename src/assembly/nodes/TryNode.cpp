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

#include "TryNode.hpp"

namespace hydro
{

TryNode::TryNode(FuncNode *owner, Label *start, Label *end, Label *jump) : _owner{owner}, _from{start}, _to{end}, _jump{jump} {}

TryNode::~TryNode() {}

void TryNode::build(Chunk *chunk)
{
    // from address
    _from->build(chunk);
    
    // emit try code
    buildChildren(chunk);
    
    // to address
    _to->build(chunk);
    
    // emit jump instruction
    chunk->bytes.put(jmp_instr);
    
    // add forward reference
    _jump->addForwardReference(chunk->bytes.position());
    
    // emit temporary address
    chunk->bytes.putInt(-1);
    
    // emit catch code
    for(CatchNode *c : _catches)
        c->build(chunk);
    
    // resolve foreward references
    _jump->build(chunk);
}

void TryNode::append(CatchNode *ctch) { _catches.push_back(ctch); }

} // namespace hydr
