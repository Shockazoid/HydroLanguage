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

#include "GetService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

hvalue GetService::global(std::string name) const { return undefined; }

hvalue GetService::property(hobject instance, std::string name) const
{
    hvalue value;
    _vm->getProperty(_env->thread()->current(), instance, name, value);
    return value;
}

bool GetService::property(hobject instance, std::string name, hvalue &value) const
{
    return _vm->getProperty(_env->thread()->current(), instance, name, value);
}

hvalue GetService::index(hobject instance, hvalue key) const
{
    hvalue value;
    _vm->getIndex(_env->thread()->current(), instance, key, value);
    return value;
}

bool GetService::index(hobject instance, hvalue key, hvalue &value) const
{
    return _vm->getIndex(_env->thread()->current(), instance, key, value);
}
    
hvalue GetService::param(hobject instance, hvalue key) const
{
    return undefined;
}

} // namespace hydro
