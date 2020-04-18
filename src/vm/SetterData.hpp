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

#ifndef __h3o_SetterData__
#define __h3o_SetterData__

#include "MethodData.hpp"

namespace hydro
{

struct SetterData : public MethodData
{
    bool automatic{false};
	struct PropertyData *property{nullptr};
};

} // namespace hydro

#endif /* __h3o_SetterData__ */
