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

#ifndef __h3o_ContextNode__
#define __h3o_ContextNode__

#include "BlockNode.hpp"
#include "ParamNode.hpp"
#include "../../vm/ContextData.hpp"

namespace hydro
{

class ContextNode : public BlockNode
{
protected:
    ContextData *_vcontext;
    std::vector<ParamNode *> _params;
    
public:
    ContextNode(ContextData *vcontext);
    virtual ~ContextNode();
    void appendParam(ParamNode *param);
    virtual void build(Chunk *chunk) override;
    ContextData *vcontext() const { return _vcontext; }
};

} // namespace hydro

#endif /* __h3o_ContextNode__ */
