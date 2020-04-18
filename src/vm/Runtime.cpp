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

#include "Runtime.hpp"

namespace hydro
{

Runtime::Runtime() : _parent{nullptr}, _definitions{} {}

Runtime::Runtime(Runtime *parent) : _parent{parent}, _definitions{} {}

Runtime::~Runtime() {}

hfunction Runtime::bind(std::string qualifiedName, hfunction function)
{
	if (find(qualifiedName))
		return nullptr; // conflict

	runtime_definition *def = new runtime_definition{};
	def->info = get_class_info(qualifiedName);
	def->content = function.tryCast<HObject>();
	_definitions.push_back(def);
	return function;
}

haction Runtime::bind(std::string qualifiedName, haction action)
{
    if (find(qualifiedName))
        return nullptr; // conflict

    runtime_definition *def = new runtime_definition{};
    def->info = get_class_info(qualifiedName);
    def->content = action.tryCast<HObject>();
    _definitions.push_back(def);
    return action;
}

hclass Runtime::bind(std::string qualifiedName, hclass clss)
{
	if (find(qualifiedName))
		return nullptr; // conflict

	runtime_definition *def = new runtime_definition{};
	def->info = get_class_info(qualifiedName);
	def->content = clss.tryCast<HObject>();
	_definitions.push_back(def);
	return clss;
}

hevent Runtime::bind(std::string qualifiedName, hevent event)
{
    if (find(qualifiedName))
        return nullptr; // conflict

    runtime_definition *def = new runtime_definition{};
    def->info = get_class_info(qualifiedName);
    def->content = event.tryCast<HObject>();
    _definitions.push_back(def);
    return event;
}

hobject Runtime::find(std::string qualifiedName)
{
	for (runtime_definition *def : _definitions)
		if (def->info->assemblyName == qualifiedName || def->info->qualifiedName == qualifiedName)
			return def->content; // success!

	// try again if parent exists
	if (_parent)
		return _parent->find(qualifiedName);

	// fail
	return nullptr;
}

hfunction Runtime::findFunction(std::string qualifiedName)
{
	for (runtime_definition *def : _definitions)
		if (hfunction clss = def->content.tryCast<HFunction>())
			if (def->info->assemblyName == qualifiedName || def->info->qualifiedName == qualifiedName)
				return clss; // success!

	// try again if parent exists
	if (_parent)
		return _parent->findFunction(qualifiedName);

	// fail
	return nullptr;
}

hclass Runtime::findClass(std::string qualifiedName)
{
	for (runtime_definition *def : _definitions)
		if (hclass clss = def->content.tryCast<HClass>())
			if (def->info->assemblyName == qualifiedName || def->info->qualifiedName == qualifiedName)
				return clss; // success!

	// try again if parent exists
	if (_parent)
		return _parent->findClass(qualifiedName);

	// fail
	return nullptr;
}


hevent Runtime::findEvent(std::string qualifiedName)
{
    for (runtime_definition *def : _definitions)
        if (hevent clss = def->content.tryCast<HEventContext>())
            if (def->info->assemblyName == qualifiedName || def->info->qualifiedName == qualifiedName)
                return clss; // success!

    // try again if parent exists
    if (_parent)
        return _parent->findEvent(qualifiedName);

    // fail
    return nullptr;
}

} // namespace hydro
