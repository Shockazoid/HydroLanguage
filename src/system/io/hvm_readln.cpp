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

#include "hvm_readln.hpp"

namespace hydro
{

std::string hvm_readln()
{
    std::string line;
    
    // read line
    getline(std::cin, line);
    
    return line;
}

} // namespace hydro
