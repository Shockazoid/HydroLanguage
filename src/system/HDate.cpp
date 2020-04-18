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

#include "HDate.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

HDate::HDate(HvmEnv *env, HClass *dateClass) : HObject{env, dateClass} {}

HDate::~HDate() {}

void HDate::construct(hvalue value)
{
    if(is_string(value))
    {
        
    }
    else if(is_number(value))
    {
        
    }
    else if(is_null(value))
    {
        
    }
}

} // namespace hydro
