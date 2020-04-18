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

#ifndef __h3o_ActionData__
#define __h3o_ActionData__

#include "FuncData.hpp"
#include "EventData.hpp"

namespace hydro
{

struct ActionData : public FuncData
{
    const EventData *context;
};

} // namespace hydro

#endif /* __h3o_ActionData__ */
