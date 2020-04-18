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

#ifndef __h3o_AutoWireContext__
#define __h3o_AutoWireContext__

namespace hydro
{

struct AutoWireContext
{
    class HvmEnv *env; // Hydro virtual environment (ffi)
    class VM *vm; // virtual machine
    struct HvmContext *cxt; // thread context
};

} // namespace hydro

#endif /* __h3o_AutoWireContext__ */
