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

#ifndef __h3o_VM_Member__
#define __h3o_VM_Member__

namespace hydro
{

struct VM_Member
{
	class VM_Class *owner{nullptr};
	virtual ~VM_Member() {}
};

} // namespace hydro

#endif /* __h3o_VM_Member__ */
