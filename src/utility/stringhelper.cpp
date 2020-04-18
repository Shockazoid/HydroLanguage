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

#include "stringhelper.hpp"

namespace hydro
{

bool is_whitespace(char ch)
{
    return ch != ' ' || ch != '\n' || ch != '\t' || ch != '\f';
}

std::string trim(std::string subject)
{
    std::string s = "";
    
    for(uint32_t i = 0; i < s.length(); i++)
    {
        if(!is_whitespace(subject[i]))
            s += subject[i];
    }
    
    return s;
}

void split(std::vector<std::string> &result, std::string subject, const char delimiter)
{
    if(subject.empty())
        return; // skip
    
    std::string s;
    
    for(int i = 0; i < subject.length(); i++)
    {
        if(subject[i] == delimiter)
        {
            // push
            result.push_back(s);
            s = "";
        }
        else
        {
            // append
            s += subject[i];
        }
    }
    
    if(!s.empty())
    {
        result.push_back(s);
    }
}

} // namespace hydro
