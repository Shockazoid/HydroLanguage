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

#include "RuntimeContext.hpp"
#include "HvmEnv.hpp"
#include "../system/HClass.hpp"

namespace hydro
{

RuntimeContext::RuntimeContext() : _ownerPackage{""}, _ownerClass{nullptr} {}

RuntimeContext::RuntimeContext(std::string ownerPackage) : _ownerPackage{ownerPackage}, _ownerClass{nullptr} {}

RuntimeContext::RuntimeContext(HClass *ownerClass) : _ownerPackage{""}, _ownerClass{ownerClass} {}

RuntimeContext::RuntimeContext(std::string ownerPackage, HClass *ownerClass) : _ownerPackage{ownerPackage}, _ownerClass{ownerClass} {}

RuntimeContext::~RuntimeContext() {}

bool RuntimeContext::enforceAccess(HvmEnv *env, uint16_t mod, RuntimeContext *callingContext)
{
	if (!callingContext)
	{
		// vm has an all access pass
		return true;
	}

	if (is_public(mod))
	{
		// global access
		return true;
	}
	else if (is_internal(mod))
	{
		// package only access
		if (!_ownerPackage.empty())
			if (_ownerPackage == callingContext->_ownerPackage)
				return true;
	}
	else if (is_protected(mod))
	{
		// subclass only access (hierarchical)
	}
	else if (is_private(mod))
	{
		// class only access
		if (_ownerClass)
			return _ownerClass == callingContext->_ownerClass;
	}
	else if (is_restricted(mod))
	{
		// vm only access
		return false;
	}
	else
	{
		// local (default)
		return true;
	}

	// error!
	return false;
}

} // namespace hydro
