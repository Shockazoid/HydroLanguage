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

#include "Compiler.hpp"
#include "Parser.hpp"

namespace hydro
{

Compiler::Compiler(Parser *parser)
{
    _lexer = parser ? parser->lexer() : nullptr;
	_parser = parser;
}

Compiler::~Compiler()
{
	delete _lexer;
	delete _parser;
}

void Compiler::clearErrors()
{
	for (CompilerError *err : _errors)
		delete err;

	_errors.clear();
}

void Compiler::clear()
{
	clearErrors();
}

void Compiler::appendError(std::string errorTxt, std::string source, uint32_t errorLine, int32_t errorIndex)
{
	CompilerError *err = new CompilerError{};
	err->text = errorTxt;
	err->line = errorLine;
	err->source = source;
	err->index = errorIndex;
	_errors.push_back(err);
}

void Compiler::appendError(std::string errorTxt, lex_token erroneousToken)
{
	CompilerError *err = new CompilerError{};
	err->text = errorTxt;
	if (erroneousToken)
	{
		err->line = erroneousToken->line();
		err->source = erroneousToken->source();
		err->index = erroneousToken->getTokenIndex();
	}
	_errors.push_back(err);
}

void Compiler::appendWarning(std::string errorTxt, std::string source, uint32_t errorLine, int32_t errorIndex)
{
	appendError(errorTxt, source, errorLine, errorIndex);
	_errors.back()->type = compiler_warning;
}

void Compiler::appendWarning(std::string errorTxt, lex_token erroneousToken)
{
	appendError(errorTxt, erroneousToken);
	_errors.back()->type = compiler_warning;
}

void Compiler::dumpErrors() const
{
	for (CompilerError *err : _errors)
	{
		std::string txt = err->text;
		std::cout << "Error in " << err->source << " on line " << err->line << "\n\t\t" << err->text << std::endl;
	}
}

Source *Compiler::source() const { return _lexer->source(); }

std::size_t Compiler::countWarnings() const
{
	std::size_t count = 0;

	for (CompilerError *err : _errors)
		if (err->type == compiler_warning)
			count++;

	return count;
}

} // namespace hydro
