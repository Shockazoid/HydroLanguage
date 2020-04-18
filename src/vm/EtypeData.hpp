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

#ifndef __h3o_EtypeData__
#define __h3o_EtypeData__

#include "VMObject.hpp"

namespace hydro
{

struct EtypeData : VMObject
{
    struct EventData *owner;
};

} // namespace hydro

#endif /* __h3o_EtypeData__ */
