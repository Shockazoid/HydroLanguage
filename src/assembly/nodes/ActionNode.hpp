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
#include "../../vm/VM_Action.hpp"

namespace hydro
{

class ActionNode : public FuncNode
{
private:
    VM_Action *_vact;
    std::string _contextName;
    
public:
    ActionNode(VM_Action *vact, std::string contextName);
    virtual ~ActionNode();
    virtual void build(Chunk *chunk) override;
    std::string contextName() const { return _contextName; }
    VM_Action *vact() const { return _vact; }
};

} // namespace hydro

#endif /* __h3o_ActionNode__ */
