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

#ifndef __h3o_EventNode__
#define __h3o_EventNode__

#include "BlockNode.hpp"
#include "ParamNode.hpp"
#include "../../vm/VM_Event.hpp"
#include "../../vm/VM_Etype.hpp"
#include "../../vm/EventParam.hpp"

namespace hydro
{

class EventNode : public BlockNode
{
public:
    EventNode(VM_Event *vevent, std::string superEventName = "");
    virtual ~EventNode();
    bool etypeExists(std::string etypeValue);
    void appendEtype(VM_Etype *etype);
    void appendParam(ParamNode *param);
    virtual void build(Chunk *chunk) override;
    VM_Event *vevent() const { return _vevent; }
    std::string superEventName() const { return _superEventName; }
protected:
    VM_Event *_vevent;
    std::string _superEventName;
    std::vector<VM_Etype *> _etypes;
    std::vector<ParamNode *> _params;
};

} // namespace hydro

#endif /* __h3o_EventNode__ */
