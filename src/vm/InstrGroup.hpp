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

#ifndef __h3o_InstrGroup__
#define __h3o_InstrGroup__

#include <cstdlib>

#include "Chunk.hpp"

namespace hydro
{

/**
 * Represents a set a instructions but may identify only one instruction but should never identify zero instructions. Grouped instructions are used by the VM to evaluate expression values that are owned by another block of code such as the default value for a function argument.
 */
struct InstrGroup
{
    Chunk *chunk;
	uint32_t startAddr{0};
	uint32_t endAddr{0};
};

} // namespace hydro

#endif /* __h3o_InstrGroup__ */
