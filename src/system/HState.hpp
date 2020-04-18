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

#ifndef __h3o_HState__
#define __h3o_HState__

#include "HObject.hpp"

namespace hydro
{

class HState final : public HObject
{
    friend class HvmEnv;
    
private:
    HObject *_stateContext;
    HState(HvmEnv *env, HClass *stateClass, HObject *stateContext);

public:
    virtual ~HState();
    HObject *stateContext() const { return _stateContext; }
};

} // namespace hydro

#endif /* __h3o_HState__ */
