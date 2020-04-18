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

#include "CastService.hpp"

namespace hydro
{

std::string CastService::string(hvalue value) const
{
    if (value.type() == typeid(HObject *))
    {
        HObject *obj = value.tryCast<HObject>();
        // TODO call string cast operator or "toString" using vm
        if (!obj)
            return "null";

        return obj->toString();
    }
    else
        return (std::string)value; // cast primitive
}

} // namespace hydro
