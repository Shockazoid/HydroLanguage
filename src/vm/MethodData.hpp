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

#ifndef __h3o_MethodData__
#define __h3o_MethodData__

#include "FuncData.hpp"
#include "MemberData.hpp"

namespace hydro
{

struct MethodData : public FuncData, public MemberData
{
	virtual ~MethodData() {}
};

} // namespace hydro

#endif /* __h3o_MethodData__ */
