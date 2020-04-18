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

#include "ParseError.hpp"

namespace hydro
{

ParseError::ParseError(std::string what, std::string source, uint32_t line) : _what{what}, _source{source}, _line{line} {}

ParseError::ParseError(std::string what) : _what{what}, _source{}, _line{0} {}

ParseError::~ParseError() {}

std::string ParseError::message() const
{
	std::string msg = "ParseError";

	if (!_source.empty())
		msg += " in " + _source;

	if (_line)
		msg += " at line " + std::to_string(_line);

	msg += ": " + _what;

	return msg;
}

} // namespace hydro
