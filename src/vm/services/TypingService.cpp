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

#include "TypingService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

bool TypingService::check(hobject type, hvalue instance) const
{
    if (hobject o = instance.tryCast<HObject>())
    {
        if (hclass c = type.tryCast<HClass>())
        {
            HClass *clss = o->context()->classOf();

            while (clss)
            {
                if (clss == c)
                    return true; // success!

                clss = clss->superClass();
            }
        }
    }

    // fail
    return false;
}

bool TypingService::check(std::string qualifiedName, hvalue instance) const
{
    return check(_env->appDomain()->system()->findClass(qualifiedName), instance);
}

} // namespace hydro
