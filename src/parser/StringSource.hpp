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

#ifndef __h3o_StringSource__
#define __h3o_StringSource__

#include <sstream>

#include "Source.hpp"

namespace hydro
{

class StringSource : public Source
{
public:
	StringSource(std::string value, std::string location = "");
	virtual ~StringSource();
};

typedef StringSource *lex_ssource;

} // namespace hydro

#endif /* __h3o_StringSource__ */
