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

#ifndef __h3o_MemberData__
#define __h3o_MemberData__

namespace hydro
{

struct MemberData
{
	class VM_Class *owner{nullptr};
	virtual ~MemberData() {}
};

} // namespace hydro

#endif /* __h3o_MemberData__ */
