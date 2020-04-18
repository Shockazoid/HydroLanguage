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

#include "HRegex.hpp"
#include "HString.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

HRegex::HRegex(HvmEnv *env, HClass *regexClass) : HObject{env, regexClass} {}

HRegex::~HRegex() {}

void HRegex::construct(hstring pattern)
{
    _pattern = _env->cast()->string(pattern);
    _value = std::regex{_pattern};
}

bool HRegex::test(hstring value)
{
    return std::regex_match(_env->cast()->string(value), _value);
}

} // namespace hydro
