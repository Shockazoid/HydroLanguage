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

#ifndef __h3o_ActionNode__
#define __h3o_ActionNode__

#include "FuncNode.hpp"
#include "../../vm/ActionData.hpp"

namespace hydro
{

class ActionNode : public FuncNode
{
private:
    ActionData *_vact;
    std::string _contextName;
    
public:
    ActionNode(ActionData *vact, std::string contextName);
    virtual ~ActionNode();
    virtual void build(Chunk *chunk) override;
    std::string contextName() const { return _contextName; }
    ActionData *vact() const { return _vact; }
};

} // namespace hydro

#endif /* __h3o_ActionNode__ */
