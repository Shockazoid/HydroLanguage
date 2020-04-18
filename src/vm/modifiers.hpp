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

#ifndef __h3o_modifiers__
#define __h3o_modifiers__

#include <cstdlib>
#include <string>

namespace hydro
{

enum h3o_modifier : uint16_t
{
	// ----- Access Modifiers -----
	mod_restricted = 1,
	mod_private = 2,
	mod_protected = 4,
	mod_internal = 8,
	mod_public = 16,

	// ----- Other Modifiers -----
	mod_abstract = 32, // makes a class, or member
	mod_native = 64,   // binds a function, method, or action to a native entity
	mod_override = 128,
	mod_dynamic = 256,
	mod_final = 512,
	mod_static = 1024,
    mod_readonly = 2048,
    mod_writeonly = 4096,
    mod_synchronized = 8192,
};

std::string get_accessor(uint16_t mask);

bool has_modifier(uint16_t mask, h3o_modifier mod);

bool is_restricted(uint16_t mask);

bool is_private(uint16_t mask);

bool is_protected(uint16_t mask);

bool is_internal(uint16_t mask);

bool is_public(uint16_t mask);

bool is_abstract(uint16_t mask);

bool is_native(uint16_t mask);

bool is_dynamic(uint16_t mask);

bool is_override(uint16_t mask);

bool is_final(uint16_t mask);

bool is_static(uint16_t mask);

bool is_readonly(uint16_t mask);

bool is_writeonly(uint16_t mask);

bool is_synchronized(uint16_t mask);

uint16_t get_modifier(std::string value);

} // namespace hydro

#endif /* __h3o_modifiers__ */
