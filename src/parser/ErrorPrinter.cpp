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

#include "ErrorPrinter.hpp"

namespace hydro
{

ErrorPrinter::ErrorPrinter(Compiler *compiler) : _compiler{compiler}, _errorsPrinted{0}, printLimit{5}, errorTemplate{"CompilationError in {source} at line {line}: {text}"}, contentPadding{0}, lineNumberPadding{0}, errorPointer{">"}, errorFill{" "}, lineDivider{" |"} {}

ErrorPrinter::~ErrorPrinter() {}

void ErrorPrinter::print()
{
	int32_t nerrors = (int32_t)_compiler->numErrors();

	for (int32_t i = 0; i < nerrors; i++)
		print(i);
}

void ErrorPrinter::print(int32_t index)
{
	// check error bounds
	if (index < 0 || index >= _compiler->numErrors())
		return; // out of bounds

	// check constraint
	if (_errorsPrinted >= printLimit)
		return; // apply constraint

	// init error data
	std::string errorSrc = _compiler->errorSourceAt(index);
	std::string errorText = _compiler->errorTextAt(index);
	int32_t errorLine = _compiler->errorLineAt(index);
	//uint32_t errorPos = _compiler->errorIndexAt(index);

	// output the error heading

	// open and output file
	std::fstream fs{};
	fs.open(errorSrc);

	if (fs.is_open())
	{
		std::string content;
		char ch;
		int32_t nlines = 0; // has at least one line
		std::string ln;

		// read file contents
		while (!fs.eof())
		{
			getline(fs, ln);
			nlines++;
			content += ln + '\n';
		}

		int32_t maxDigits = countDigits(nlines);
		int32_t n = maxDigits + lineNumberPadding, m = n;

		// draw document header
		std::cout << std::endl; // skip line

		// output source file
		std::cout << "     ";
		while (n-- > 0)
			std::cout << " ";
		std::cout << getSourceName(errorSrc) << std::endl;

		std::cout << "     ";
		while (n-- > 0)
			std::cout << " ";

		std::cout << std::endl;

		std::cout << "  "; // empty > (the error pointer)
		while (m-- > 0)
			std::cout << " ";			   // padding
		std::cout << lineDivider << std::endl; // divider

		// draw file
		for (int32_t line = 1, i = 0; line <= nlines; i++)
		{
			int32_t lnDigits = countDigits(line);
			int32_t tab = maxDigits + contentPadding;
			int32_t lnPadding = maxDigits - lnDigits + lineNumberPadding;
			std::string fill = line == errorLine ? errorFill : " ";

			// draw padding (makes all digits fit)
			while (lnPadding-- > 0)
				std::cout << " ";

			// draw error pointer or space
			if (line == errorLine)
				std::cout << errorPointer << fill;
			else
				std::cout << fill << fill;

			// draw line #
			std::cout << line;

			// draw divider
			std::cout << fill << lineDivider;

			// draw tab
			while (tab-- > 0)
				std::cout << fill;

			for (; i < content.length(); i++)
			{
				ch = content[i];
				if (ch == '\n')
				{

					if (line == errorLine)
						std::cout << fill << fill << fill << fill << fill << " [" << errorText << "]" << std::endl;
					else
						std::cout << std::endl;
					line++;
					break;
				}

				std::cout << ch;
			}
		}

		n = maxDigits + lineNumberPadding;

		// draw document footer
		std::cout << "  "; // empty > (the error pointer)
		while (n-- > 0)
			std::cout << " ";			   // padding
		std::cout << lineDivider << std::endl; // divider
	}

	// count printed errors
	_errorsPrinted++;
}

} // namespace hydro
