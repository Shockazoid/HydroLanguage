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

#ifndef __h3o_Socket__
#define __h3o_Socket__

#include "../HObject.hpp"

namespace hydro
{

class Socket : public HObject
{
    template <typename X>
    friend struct NativeClassAllocator;
    
private:
    Socket(HvmEnv *env, HClass *socketClass);
    
    void construct();
    
public:
    virtual ~Socket();
};

} // namespace hydro

#endif /* __h3o_Socket__ */
