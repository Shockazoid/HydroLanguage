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

#include "modifiers.hpp"

namespace hydro
{

std::string get_accessor(uint16_t mask)
{
	if (is_public(mask))
		return "public";
	else if (is_internal(mask))
		return "internal";
	else if (is_protected(mask))
		return "protected";
	else if (is_private(mask))
		return "private";
	else if (is_restricted(mask))
		return "restricted";
	return "";
}

bool has_modifier(uint16_t mask, h3o_modifier mod)
{
	return (mask & ((uint16_t)mod)) != 0;
}

bool is_restricted(uint16_t mask)
{
	return (mask & ((uint16_t)mod_restricted)) != 0;
}

bool is_private(uint16_t mask)
{
	return (mask & ((uint16_t)mod_private)) != 0;
}

bool is_protected(uint16_t mask)
{
	return (mask & ((uint16_t)mod_protected)) != 0;
}

bool is_internal(uint16_t mask)
{
	return (mask & ((uint16_t)mod_internal)) != 0;
}

bool is_public(uint16_t mask)
{
	return (mask & ((uint16_t)mod_public)) != 0;
}

bool is_abstract(uint16_t mask)
{
	return (mask & ((uint16_t)mod_abstract)) != 0;
}

bool is_native(uint16_t mask)
{
	return (mask & ((uint16_t)mod_native)) != 0;
}

bool is_dynamic(uint16_t mask)
{
	return (mask & ((uint16_t)mod_dynamic)) != 0;
}

bool is_override(uint16_t mask)
{
	return (mask & ((uint16_t)mod_override)) != 0;
}

bool is_final(uint16_t mask)
{
	return (mask & ((uint16_t)mod_final)) != 0;
}

bool is_static(uint16_t mask)
{
	return (mask & ((uint16_t)mod_static)) != 0;
}

bool is_readonly(uint16_t mask)
{
    return (mask & ((uint16_t)mod_readonly)) != 0;
}

bool is_writeonly(uint16_t mask)
{
    return (mask & ((uint16_t)mod_writeonly)) != 0;
}

bool is_synchronized(uint16_t mask)
{
    return (mask & ((uint16_t)mod_synchronized)) != 0;
}

uint16_t get_modifier(std::string value)
{
	if (value == "restricted")
		return (uint16_t)mod_restricted;
	else if (value == "private")
		return (uint16_t)mod_private;
	else if (value == "protected")
		return (uint16_t)mod_protected;
	else if (value == "internal")
		return (uint16_t)mod_internal;
	else if (value == "public")
		return (uint16_t)mod_public;
	else if (value == "abstract")
		return (uint16_t)mod_abstract;
	else if (value == "native")
		return (uint16_t)mod_native;
	else if (value == "dynamic")
		return (uint16_t)mod_dynamic;
	else if (value == "override")
		return (uint16_t)mod_override;
	else if (value == "final")
		return (uint16_t)mod_final;
	else if (value == "static")
		return (uint16_t)mod_static;
    else if (value == "readonly")
        return (uint16_t)mod_readonly;
    else if (value == "writeonly")
        return (uint16_t)mod_writeonly;
    else if (value == "writeonly")
        return (uint16_t)mod_writeonly;
    else if (value == "synchronized")
        return (uint16_t)mod_synchronized;
	return 0;
}

} // namespace hydro
