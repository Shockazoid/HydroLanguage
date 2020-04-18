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

#ifndef __h3o_ConstructorResource__
#define __h3o_ConstructorResource__

#include "NativeResource.hpp"
#include "../glue.hpp"

namespace hydro
{

template <typename ClassType>
class ConstructorResource : public NativeFunctionResource
{
	template <typename X>
	friend class ClassResource;

private:
	function_glue *_glue;

	template <typename... Arg>
	ConstructorResource(void (*constructor)(Arg...)) : NativeFunctionResource{""}, _glue{new native_function{constructor}} {}
    
    template <typename... Arg>
    ConstructorResource(void (ClassType::*constructor)(Arg...)) : NativeFunctionResource{""}, _glue{new native_method{constructor}} {}
public:
	virtual ~ConstructorResource() {}
	function_glue *glue() const override { return _glue; }

};

} // namespace hydro

#endif /* __h3o_ConstructorResource__ */
