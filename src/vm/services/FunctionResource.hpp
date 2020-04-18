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

#ifndef __h3o_FunctionResource__
#define __h3o_FunctionResource__

#include "NativeResource.hpp"
#include "../glue.hpp"

namespace hydro
{

template <typename ReturnType, typename... Arg>
class FunctionResource : public NativeFunctionResource
{
    friend class NativeResourceRegistry;
    friend struct NativeService;

private:
	function_glue *_glue;

	FunctionResource(std::string qualifiedName, ReturnType (*fn)(Arg...)) : NativeFunctionResource(qualifiedName), _glue{new native_function{fn}} {}

public:
    virtual ~FunctionResource() {}
	function_glue *glue() const override { return _glue; }
};

} // namespace hydro

#endif /* __h3o_FunctionResource__ */
