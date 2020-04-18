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

#ifndef __h3o_stringhelper__
#define __h3o_stringhelper__

#include <string>
#include <vector>

namespace hydro
{

bool is_whitespace(char ch);

std::string trim(std::string subject);

void split(std::vector<std::string> &result, std::string subject, const char delimiter = ' ');

} // namespace hydro

#endif /* __h3o_stringhelper__ */
