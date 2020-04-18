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

#ifndef __h3o_classhelper__
#define __h3o_classhelper__

#include <regex>
#include <string>

namespace hydro
{
static const std::regex assemblyNamePattern = std::regex("^([a-zA-Z0-9_-]+\\/)*([a-zA-Z_][a-zA-Z0-9_]*)$");

static const std::regex qualifiedNamePattern = std::regex("^([a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)*::)?([a-zA-Z_][a-zA-Z0-9_]*)$");

struct class_info
{
	std::string name;
	std::string qualifiedName;
	std::string packageName;
	std::string assemblyName;
};

class_info *get_class_info(std::string name);

std::string get_assembly_name(std::string name);

std::string get_qualified_name(std::string name);

std::string get_package_name(std::string name);

std::string get_simple_name(std::string name);

} // namespace hydro

#endif /* __h3o_classhelper__ */
