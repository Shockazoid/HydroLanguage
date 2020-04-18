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

#ifndef __h3o_BytecodeAssembler__
#define __h3o_BytecodeAssembler__

#include <vector>
#include "Parser.hpp"
#include "BytecodeDefinition.hpp"

namespace hydro
{

class BytecodeAssembler : protected Parser
{
protected:
	std::vector<BytecodeDefinition *> _definitions;

	void registerOpcode(std::string name, uint8_t opcode);
	uint8_t readOpcode();

public:
	BytecodeAssembler(lex_lexer lexer);
	virtual ~BytecodeAssembler();
	void program(std::string assembly);
};

} // namespace hydro

#endif /* __h3o_BytecodeAssembler__ */
