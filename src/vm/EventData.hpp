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

#ifndef __h3o_EventData__
#define __h3o_EventData__

#include "VMObject.hpp"
#include "MemberData.hpp"
#include "EtypeData.hpp"
#include "ContextParam.hpp"

namespace hydro
{

struct EventData : public VMObject
{
    const EventData *base;
	EtypeData **etypes;
    uint16_t netypes;
    uint16_t nparams;
    ContextParam **params;
};

} // namespace hydro

#endif /* __h3o_EventData__ */
