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

#ifndef __h3o_CatchNode__
#define __h3o_CatchNode__

#include "BlockNode.hpp"
#include "TypeNode.hpp"
#include "Label.hpp"
#include "../../vm/ErrorHandler.hpp"
#include "../../vm/opcodes.hpp"
#include "../../vm/NilTypeSpecifier.hpp"

namespace hydro
{

class CatchNode : public BlockNode
{
private:
    int32_t _slot;
    TypeNode *_type;
    Label *_from;
    Label *_to;
    Label *_jump;
    ErrorHandler *_errorHandler;
    
public:
    CatchNode(int32_t slot, TypeNode *type, Label *from, Label *to, Label *jump);
    virtual ~CatchNode();
    virtual void build(Chunk *chunk);
    ErrorHandler *errorHandler() const { return _errorHandler; }
};

} // namespace hydro

#endif /* __h3o_CatchNode__ */
