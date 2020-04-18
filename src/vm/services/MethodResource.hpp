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

#ifndef __h3o_MethodResource__
#define __h3o_MethodResource__

#include "NativeResource.hpp"
#include "../glue.hpp"

namespace hydro
{

template <typename ClassType>
class MethodResource final : public NativeFunctionResource
{
	template <typename X>
	friend class ClassResource;

private:
	function_glue *_glue;

	template <typename ReturnType, typename... Arg>
	MethodResource(std::string name, ReturnType (ClassType::*method)(Arg...)) : NativeFunctionResource{name}, _glue{new native_method{method}} {}

	template <typename ReturnType, typename... Arg>
	MethodResource(std::string name, ReturnType (*method)(Arg...)) : NativeFunctionResource{name}, _glue{new native_method{method}} {}

public:
	virtual ~MethodResource() {}
	function_glue *glue() const override { return _glue; }
};

} // namespace hydro

#endif /* __h3o_MethodResource__ */
