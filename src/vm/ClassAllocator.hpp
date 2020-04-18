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

#ifndef __h3o_ClassAllocator__
#define __h3o_ClassAllocator__

namespace hydro
{

struct ClassAllocator
{
    virtual HObject *alloc(HvmEnv *env, HClass *instanceClass) const = 0;
};

template <typename T>
struct NativeClassAllocator final : public ClassAllocator
{
    virtual HObject *alloc(HvmEnv *env, HClass *instanceClass) const
    {
        // allocate instance
        return new T{env, instanceClass};
    }
};

} // namespace hydro

#endif /* __h3o_ClassAllocator__ */
