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

#ifndef __h3o_Source__
#define __h3o_Source__

#include <iostream>
#include <string>

namespace hydro
{

class Source
{
protected:
	std::istream &_inputStream;
	std::string _location;
    
	Source(std::istream &input, std::string location = "");

public:
	virtual ~Source();
	virtual char nextChar();
	char currentChar() const;
	virtual bool eof() const;
	std::string location() const;
	bool hasNext() const;
};

typedef Source *lex_source;

} // namespace hydro

#endif /* __h3o_Source__ */
