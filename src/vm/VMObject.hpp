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

#ifndef __h3o_VMObject__
#define __h3o_VMObject__

#include <string>

namespace hydro
{

struct VMObject
{
	uint16_t modifier;
	std::string name;
	virtual ~VMObject() {}
};

} // namespace hydro

#endif /* __h3o_VMObject__ */
