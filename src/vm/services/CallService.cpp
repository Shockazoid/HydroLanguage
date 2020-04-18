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

#include "CallService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

hvalue CallService::target(hvalue callable, std::list<hvalue> &args, hvalue thisObject) const
{
    hvalue result;
    _vm->invokeTarget(_env->thread()->current(), callable, args, result, thisObject);
    return result;
}

hvalue CallService::global(std::string, std::list<hvalue> &args) const
{
    return nullptr;
}

hvalue CallService::property(hobject instance, std::string name, std::list<hvalue> &args) const
{
    return nullptr;
}


hvalue CallService::index(hobject instance, hvalue key, std::list<hvalue> &args) const
{
    //return _vm->callIndex();
    return undefined;
}



} // namespace hydro
