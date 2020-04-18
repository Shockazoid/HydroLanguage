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

#ifndef __h3o_HvmDelegate__
#define __h3o_HvmDelegate__

#include "HvmEnv.hpp"

namespace hydro
{

class HvmDelegate
{
protected:
    HvmDelegate(HvmInitOptions *options)
    {
        if(HvmEnv::_instance)
            throw std::runtime_error{"The HvmEnv has already been initialized."};
        
        // create HvmEnv
        HvmEnv *env = new HvmEnv{this};
        env->setup(options);
    }
    
public:
    virtual ~HvmDelegate() {}
    virtual void setup(class HvmEnv *env) = 0;
    virtual void registerNative(HvmEnv *env) = 0;
    virtual void startUp(HvmEnv *env) = 0;
    virtual void shutDown(HvmEnv *env) = 0;
};

} // namespace hydro

#endif /* __h3o_HvmDelegate__ */
