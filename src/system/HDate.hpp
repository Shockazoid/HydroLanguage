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

#ifndef __h3o_HDate__
#define __h3o_HDate__

#include "HObject.hpp"

namespace hydro
{

class HDate final : public HObject
{
    friend class HvmEnv;
    template <typename X>
    friend struct NativeClassAllocator;
    
private:
    
    HDate(HvmEnv *env, HClass *dateClass);
    void construct(hvalue value);

public:
    virtual ~HDate();
};

typedef object_ptr<HDate> hdate;

} // namespace hydro

#endif /* __h3o_Date__ */
