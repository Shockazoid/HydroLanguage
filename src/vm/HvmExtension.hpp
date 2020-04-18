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

#ifndef __h3o_HvmExtension__
#define __h3o_HvmExtension__

#include <memory>

namespace hydro
{

template <typename T>
class HvmExtension final
{
    friend class HvmEnv;
    
private:
    T *t;
    
public:
    HvmExtension() : t{nullptr} {}
    HvmExtension(const HvmExtension &service) : t{service.t} {}
    HvmExtension(HvmExtension &&service) : t{std::move(service)} {}
    HvmExtension(HvmEnv *env) : t{new T{env}} {}
    ~HvmExtension() {}
    T *operator->() const { return t; }
    HvmExtension &operator=(HvmExtension &rhs)
    {
        t = rhs.t;
        return *this;
    }
    HvmExtension &operator=(HvmExtension &&rhs)
    {
        t = std::move(rhs.t);
        return *this;
    }
};

} // namespace hydro

#endif /* __h3o_HvmExtension__ */
