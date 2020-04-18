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

#ifndef __h3o_ParamNode__
#define __h3o_ParamNode__

#include "H3oNode.hpp"
#include "InstrGroupNode.hpp"
#include "../../vm/EventParam.hpp"

namespace hydro
{

class ParamNode : public H3oNode
{
private:
    EventParam *_vparam;
    InstrGroupNode *_defaultValue;
    
public:
    ParamNode(EventParam *vparam, InstrGroupNode *defaultValue = nullptr);
    virtual ~ParamNode();
    virtual void build(Chunk *chunk) override;
    EventParam *vparam() const { return _vparam; }
};

} // namespace hydro

#endif /* __h3o_ParamNode__ */
