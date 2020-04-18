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

#include "HError.hpp"

namespace hydro
{

HError::HError(HvmEnv *env, HClass *errorClass) : HObject{env, errorClass} {}

HError::~HError() {}

void HError::construct(std::string message, int32_t code)
{
    _message = message;
    _code = code;
}

std::string HError::getStackTrace()
{
    
    return "";
}

} // namespace hydro
