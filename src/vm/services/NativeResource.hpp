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

#ifndef __h3o_NativeResource__
#define __h3o_NativeResource__

#include <string>
#include <stdexcept>
#include <vector>
#include <type_traits>

#include "../../system/HObject.hpp"
#include "../../utility/classhelper.hpp"
#include "../VM_Object.hpp"
#include "../glue.hpp"
#include "../ClassAllocator.hpp"

namespace hydro
{

enum native_resource_type : uint8_t
{
    class_resource,
    constructor_resource,
    property_resource,
    method_resource,
    function_resource
};

class NativeResource
{
protected:
	class_info *_info;

	NativeResource(std::string qualifiedName) : _info{get_class_info(qualifiedName)} {}

public:
	virtual ~NativeResource() {}
	virtual const std::type_info &type() const { return typeid(void); }
	std::string name() const { return _info->name; }
	std::string assemblyName() const { return _info->assemblyName; }
	std::string qualifiedName() const { return _info->qualifiedName; }
	std::string packageName() const { return _info->packageName; }
};

class NativeFunctionResource : public NativeResource
{
public:
    NativeFunctionResource(std::string qualifiedName) : NativeResource{qualifiedName} {}
    virtual ~NativeFunctionResource() {}
    virtual function_glue *glue() const = 0;
};

class NativePropertyResource : public NativeResource
{
public:
    NativePropertyResource(std::string qualifiedName) : NativeResource{qualifiedName} {}
    virtual ~NativePropertyResource() {}
    virtual property_glue *glue() const = 0;
    virtual NativeFunctionResource *getter() const = 0;
    virtual void getter(NativeFunctionResource *getter) = 0;
    virtual NativeFunctionResource *setter() const = 0;
    virtual void setter(NativeFunctionResource *setter) = 0;
};

class NativeClassResource : public NativeResource
{
public:
    NativeClassResource(std::string qualifiedName) : NativeResource{qualifiedName} {}
    virtual ~NativeClassResource() {}
    virtual NativePropertyResource *lookupProperty(std::string name) = 0;
    virtual NativeFunctionResource *lookupMethod(std::string name) = 0;
    virtual ClassAllocator *allocator() const = 0;
    virtual NativeFunctionResource *constructor() const = 0;
};

} // namespace hydro

#endif /* __h3o_NativeResource__ */
