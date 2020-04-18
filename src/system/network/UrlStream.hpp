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

#ifndef __h3o_UrlStream__
#define __h3o_UrlStream__

#include "Url.hpp"

namespace hydro
{

class UrlStream : public HObject
{
    friend class HvmEnv;
    template <typename X>
    struct ClassAllocator;
    
private:
    UrlStream(HvmEnv *env, HClass *urlStreamClass);
    
public:
    virtual ~UrlStream();
    void close();
};

} // namespace hydro

#endif /* __h3o_UrlStream__ */
