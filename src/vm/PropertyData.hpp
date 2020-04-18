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

#ifndef __h3o_PropertyData__
#define __h3o_PropertyData__

#include <string>
#include <cstdlib>

#include "VMObject.hpp"
#include "MemberData.hpp"
#include "TypeSpecifier.hpp"
#include "GetterData.hpp"
#include "SetterData.hpp"

namespace hydro
{

struct PropertyData : public VMObject, public MemberData
{
	std::string name{""};
	TypeSpecifier *type{nullptr};
	GetterData *getter{nullptr};
	SetterData *setter{nullptr};
    InstrGroup *initValue{nullptr};
	virtual ~PropertyData() {}
};

} // namespace hydro

#endif /* __h3o_PropertyData__ */
