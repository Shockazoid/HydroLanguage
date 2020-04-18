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

#ifndef __h3o_TryNode__
#define __h3o_TryNode__

#include "FuncNode.hpp"
#include "CatchNode.hpp"
#include "Label.hpp"

namespace hydro
{

class TryNode : public BlockNode
{
private:
    FuncNode *_owner;
    Label *_from;
    Label *_to;
    Label *_jump;
    std::vector<CatchNode *> _catches;
    
public:
    TryNode(FuncNode *owner, Label *start, Label *end, Label *jump);
    virtual ~TryNode();
    virtual void build(Chunk *chunk) override;
    void append(CatchNode *ctch);
    const std::vector<CatchNode *> catches() const { return _catches; }
};
    
} // namespace hydro

#endif /* __h3o_TryNode__ */
