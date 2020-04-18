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

#ifndef __h3o_BytecodeDefinition__
#define __h3o_BytecodeDefinition__

#include <string>
#include <cstdlib>

namespace hydro
{

struct BytecodeDefinition
{
	std::string name;
	uint8_t opcode;
	BytecodeDefinition(std::string n, uint8_t o) : name{n}, opcode{o} {}
	~BytecodeDefinition() {}
};

} // namespace hydro

#endif /* __h3o_BytecodeDefinition__ */
