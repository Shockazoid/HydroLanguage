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

#include "ThrowService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

void ThrowService::target(hvalue err) const
{
    _vm->throwError(_env->thread()->current(), err);
}

void ThrowService::error(std::string text, int32_t code) const
{
    hobject err = _env->create(_env->core()->ErrorClass(), text, code);
    target(err); // throw error
}

bool ThrowService::errorThrown() const
{
    return _env->error()->pending();
}

} // namespace hydro
