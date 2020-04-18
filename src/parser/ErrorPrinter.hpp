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

#ifndef __h3o_ErrorPrinter__
#define __h3o_ErrorPrinter__

#include <fstream>

#include "Compiler.hpp"

namespace hydro
{

class ErrorPrinter
{
protected:
	Compiler *_compiler;
	uint32_t _errorsPrinted;

	static int32_t countDigits(int32_t n)
	{
		int32_t count = 0;
		while (n != 0)
		{
			n = n / 10;
			++count;
		}
		return count;
	}

	static std::string getSourceName(std::string source)
	{
		const auto n = source.length();
		std::string name;

		for (auto i = 0; i < n; i++)
		{
			char ch = source[i];

			if (ch == '/')
			{
				// clear and try again
				name = "";
			}
			else
			{
				// concat
				name += ch;
			}
		}

		return name;
	}

public:
	uint32_t printLimit;
	std::string errorTemplate;
	uint32_t contentPadding;
	uint32_t lineNumberPadding;
	std::string errorPointer;
	std::string errorFill;
	std::string lineDivider;
	ErrorPrinter(Compiler *compiler);
	virtual ~ErrorPrinter();
	virtual void print();
	virtual void print(int32_t errorIndex);
};

} // namespace hydro

#endif /* __h3o_ErrorPrinter__ */
