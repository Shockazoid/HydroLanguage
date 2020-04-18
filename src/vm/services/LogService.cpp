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

#include "LogService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{
    // TODO use adapter pattern to allow for customization rather than just using iostream
    
    void LogService::print(hvalue value) const
    {
        // TODO call adapter::print() instead
        std::cout << _env->cast()->string(value) << std::endl;
    }

    void LogService::error(hvalue value) const
    {
        // TODO call adapter::error() instead
        std::cerr << _env->cast()->string(value) << std::endl;
    }

    void LogService::clear() const
    {
        // TODO call adapter::clear()
    }

} // namespace hydro
