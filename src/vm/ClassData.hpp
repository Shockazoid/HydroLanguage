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

#ifndef __h3o_ClassData__
#define __h3o_ClassData__

#include <string>
#include <cstdlib>

#include "VMObject.hpp"
#include "Chunk.hpp"
#include "MemberData.hpp"

namespace hydro
{

struct ClassData : public VMObject
{
    Chunk *chunk;
	const ClassData *base{nullptr};
	uint16_t nfields;
    uint16_t nstatic;
	uint16_t ninstance;
	VMObject **fields;
	struct ConstructorData *construct;
	virtual ~ClassData() {}
};

} // namespace hydro

#endif /* __h3o_ClassData__ */
