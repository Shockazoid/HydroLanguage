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

#include "UtilityService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

std::string UtilityService::qualifiedName(const hvalue &value) const
{

    if (is_null(value))
        return "null";
    else if (is_object(value))
    {
        hobject obj = value;
        HClass *clss = obj->context()->classOf();
        return get_qualified_name(clss->assemblyName());
    }
    else if (is_bool(value))
        return "bool";
    else if (is_char(value))
        return "char";
    else if (is_byte(value))
        return "byte";
    else if (is_sbyte(value))
        return "sbyte";
    else if (is_ushort(value))
        return "ushort";
    else if (is_short(value))
        return "short";
    else if (is_uint(value))
        return "uint";
    else if (is_int(value))
        return "int";
    else if (is_ulong(value))
        return "ulong";
    else if (is_long(value))
        return "long";
    else if (is_float(value))
        return "float";
    else if (is_double(value))
        return "double";
    else if (is_undefined(value))
        return "undefined";

    // unknown (possibly a closure)
    return "";
}


std::string UtilityService::describe(const hvalue &val) const
{
    if (is_undefined(val))
        return "undefined";
    else if (is_null(val))
        return "null";
    else if (is_object(val))
        return "object of type \"" + qualifiedName(val) + "\"";
    else if (is_primitive(val))
        return "primitive type \"" + qualifiedName(val) + "\"";

    return qualifiedName(val);
}


} // namespace hydro
