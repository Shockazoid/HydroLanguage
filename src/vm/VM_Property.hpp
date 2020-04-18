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

#ifndef __h3o_VM_Property__
#define __h3o_VM_Property__

#include <string>
#include <cstdlib>

#include "VMObject.hpp"
#include "VM_Member.hpp"
#include "TypeSpecifier.hpp"
#include "GetterData.hpp"
#include "SetterData.hpp"

namespace hydro
{

struct VM_Property : public VMObject, public VM_Member
{
	std::string name{""};
	TypeSpecifier *type{nullptr};
	GetterData *getter{nullptr};
	SetterData *setter{nullptr};
    InstrGroup *initValue{nullptr};
	virtual ~VM_Property() {}
};

} // namespace hydro

#endif /* __h3o_VM_Property__ */