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

#include "BytecodeAssembler.hpp"
#include "StringSource.hpp"

namespace hydro
{

BytecodeAssembler::BytecodeAssembler(lex_lexer lexer) : Parser{lexer}, _definitions{}
{
}

BytecodeAssembler::~BytecodeAssembler()
{
	for (BytecodeDefinition *def : _definitions)
		delete def;

	_definitions.clear();
}

void BytecodeAssembler::registerOpcode(std::string name, uint8_t opcode)
{
	_definitions.push_back(new BytecodeDefinition{name, opcode});
}

uint8_t BytecodeAssembler::readOpcode()
{
	std::string name = tokenVal();

	for (BytecodeDefinition *def : _definitions)
		if (def->name == name)
			return def->opcode; // success!

	// fail
	abrupt();
	return 0; // satisfy compiler
}

void BytecodeAssembler::program(std::string assembly)
{
	Parser::parse(new StringSource{assembly, "(assembly source)"});
}

} // namespace hydro
