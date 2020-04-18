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

#ifndef __h3o_HThread__
#define __h3o_HThread__

#include "HObject.hpp"

namespace hydro
{

class HThread : public HObject
{
    friend class HvmEnv;
    template <typename X>
    friend struct NativeClassAllocator;
    friend class VM;
    
protected:
    HThread(HvmEnv *env, HClass *threadClass);
    
    void construct();
    
public:
    virtual ~HThread();
};

} // namespace hydro

#endif /* __h3o_HThread__ */
