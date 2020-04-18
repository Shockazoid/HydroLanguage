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

#ifndef __h3o_HContext__
#define __h3o_HContext__

#include "HObject.hpp"
#include "../vm/ContextData.hpp"

namespace hydro
{

class HContext : public HObject
{
    friend class HvmEnv;
    friend class VM;
    
protected:
    const ContextData *_vcontext;
    HContext(HvmEnv *env, HClass *contextClass, const ContextData *vcontext);
    virtual void setup();
    
public:
    virtual ~HContext();
    const ContextData *vcontext() const { return _vcontext; }
    virtual std::string toString() override
    {
        return "[Context " + get_simple_name(_vcontext->name) + "]";
    }
};

typedef object_ptr<HContext> hcontext;

} // namespace hydro

#endif /* __h3o_Context__ */
