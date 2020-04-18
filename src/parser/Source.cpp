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

#include "Source.hpp"

namespace hydro
{

Source::Source(std::istream &input, std::string location) : _inputStream{input}, _location{location} {}

Source::~Source() { delete &_inputStream; }

char Source::nextChar()
{
	if (_inputStream.eof())
        return EOF;

    char ch;
    ch = _inputStream.get(); // read char
    return ch;
}

bool Source::eof() const { return _inputStream.eof(); }

std::string Source::location() const { return _location; }

bool Source::hasNext() const
{
	return !_inputStream.eof();
}

} // namespace hydro
