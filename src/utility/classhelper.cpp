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

#include <vector>
#include <sstream>

#include "classhelper.hpp"

namespace hydro
{

class_info *get_class_info(std::string name)
{
	class_info *info = nullptr;

	if (std::regex_match(name, assemblyNamePattern))
	{
		info = new class_info{};
		info->assemblyName = name;
		std::stringstream ss(info->assemblyName);
		std::string token;
		std::vector<std::string> tokens;
		const int n = (int)std::count(info->assemblyName.begin(), info->assemblyName.end(), '/') + 1;
		int i = 0;

		while (std::getline(ss, token, '/'))
		{
			i++;

			if (i < n)
			{
				info->packageName += ((info->packageName == "") ? token : "." + token);
				info->qualifiedName += ((info->qualifiedName == "") ? token : "." + token);
			}
			else
			{
				info->name = token;
				info->qualifiedName += ((info->qualifiedName == "") ? info->name : "::" + info->name);
			}
		}
	}
	else if (std::regex_match(name, qualifiedNamePattern))
	{
		info = new class_info{};
		info->qualifiedName = name;
		bool shift = false;
		int len = (int)info->qualifiedName.length();

		for (int i = 0; i < len; i++)
		{
			if (info->qualifiedName[i] == ':')
			{
				i++; // skip :
				shift = true;
				continue;
			}
			else if (shift)
				info->name += info->qualifiedName[i];
			else
				info->packageName += info->qualifiedName[i];
		}

		if (!shift)
		{
			info->name = info->assemblyName = info->packageName;
			info->packageName = "";
		}

		len = (int)info->packageName.length();

		if (len > 0)
		{
			// generate assembly name
			for (int i = 0; i < len; i++)
			{
				if (info->packageName[i] == '.')
					info->assemblyName += "/";
				else
					info->assemblyName += info->packageName[i];
			}

			info->assemblyName += "/";
		}

		info->assemblyName += info->name;
	}

	// invalid
	return info;
}

std::string get_assembly_name(std::string name)
{
	class_info *info = get_class_info(name);

	if (info)
	{
		std::string asmName = info->assemblyName;
		delete info;
		return asmName;
	}

	return "";
}

std::string get_qualified_name(std::string name)
{
	class_info *info = get_class_info(name);

	if (info)
	{
		std::string qname = info->qualifiedName;
		delete info;
		return qname;
	}

	// invalid
	return "";
}

std::string get_package_name(std::string name)
{
	class_info *info = get_class_info(name);

	if (info)
	{
		std::string pkg = info->packageName;
		delete info;
		return pkg;
	}

	// invalid
	return "";
}

std::string get_simple_name(std::string name)
{
	class_info *info = get_class_info(name);

	if (info)
	{
		std::string name = info->name;
		delete info;
		return name;
	}

	// invalid
	return "";
}

} // namespace hydro
