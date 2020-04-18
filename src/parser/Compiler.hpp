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

#ifndef __h3o_Compiler__
#define __h3o_Compiler__

#include <string>
#include <cstdlib>
#include <vector>

#include "Token.hpp"

namespace hydro
{

enum compiler_error_type
{
	compiler_error,
	compiler_warning
};

struct CompilerError
{
	compiler_error_type type{compiler_error};
	std::string text{""};
	std::string source{"[unknown source]"};
	uint32_t line{0};
	int32_t index{-1};
};

class Compiler
{
protected:
	std::vector<CompilerError *> _errors{};
	class Lexer *_lexer;
	class Parser *_parser;

	void clearErrors();

	Compiler(Parser *parser = nullptr);

public:
	virtual ~Compiler();
	void clear();
	virtual void compile(Source *src) = 0;
	virtual void compile(std::string data) = 0;
	virtual bool compileFile(std::string path) = 0;
	void appendError(std::string errorTxt, std::string source, uint32_t errorLine, int32_t errorIndex);
	void appendError(std::string errorTxt, lex_token erroneousToken = nullptr);
	void appendWarning(std::string errorTxt, std::string source, uint32_t errorLine, int32_t errorIndex);
	void appendWarning(std::string errorTxt, lex_token erroneousToken = nullptr);
	compiler_error_type errorTypeAt(uint32_t index) const { return _errors[index]->type; }
	std::string errorSourceAt(uint32_t index) const { return _errors[index]->source; }
	std::string errorTextAt(uint32_t index) const { return _errors[index]->text; }
	uint32_t errorLineAt(uint32_t index) const { return _errors[index]->line; }
	int32_t errorIndexAt(uint32_t index) const { return _errors[index]->index; }
	virtual void dumpErrors() const;
	class Source *source() const;
	std::size_t numErrors() const { return _errors.size(); }
	std::size_t countWarnings() const;
	bool failed() const { return numErrors() > 0 && countWarnings() < numErrors(); }
};

} // namespace hydro

#endif /* __h3o_Compiler__ */
