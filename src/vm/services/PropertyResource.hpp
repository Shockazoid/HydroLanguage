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

#ifndef __h3o_PropertyResource__
#define __h3o_PropertyResource__

#include "NativeResource.hpp"
#include "../glue.hpp"

namespace hydro
{

class GetterResource final : public NativeFunctionResource
{
    template <typename X>
    friend class ClassResource;
    
private:
	function_glue *_glue;

	// native getter (function)
	template <typename PropertyType, typename... Arg>
	GetterResource(PropertyType (*getter)(Arg...)) : NativeFunctionResource{""}, _glue{new native_function{getter}} {}

	// native getter (method)
	template <typename ClassType, typename PropertyType, typename... Arg>
	GetterResource(PropertyType (ClassType::*getter)(Arg...)) : NativeFunctionResource{""}, _glue{new native_method{getter}} {}

public:
	virtual ~GetterResource() {}
	function_glue *glue() const override { return _glue; }
};

class SetterResource final : public NativeFunctionResource
{
    template <typename X>
    friend class ClassResource;

private:
	function_glue *_glue;

	// native setter (function)
	template <typename PropertyType, typename... Arg>
	SetterResource(void (*setter)(PropertyType, Arg...)) : NativeFunctionResource{""}, _glue{new native_function{setter}} {}

	// native setter (method)
	template <typename ClassType, typename PropertyType, typename... Arg>
	SetterResource(void (ClassType::*setter)(PropertyType, Arg...)) : NativeFunctionResource{""}, _glue{new native_method{setter}} {}

public:
	virtual ~SetterResource() {}
	function_glue *glue() const override { return _glue; }
};

//template <typename ClassType, typename PropertyType>
class PropertyResource final : public NativePropertyResource
{
	template <typename X>
	friend class ClassResource;

private:
	property_glue *_glue{nullptr};
	NativeFunctionResource *_getter{nullptr};
	NativeFunctionResource *_setter{nullptr};
    
    // native property
    PropertyResource(std::string name) : NativePropertyResource{name} {}
    
	// native property
    template <typename ClassType, typename PropertyType>
    PropertyResource(std::string name, PropertyType (ClassType::*property)) : NativePropertyResource{name}, _glue{new native_property{property}}, _getter{nullptr}, _setter{nullptr} {}
    /*
    // native getter/setter
    PropertyResource(std::string name, GetterResource *getter, SetterResource *setter) : NativePropertyResource{name}, _glue{nullptr}, _getter{getter}, _setter{setter} {}
    
	// native getter
	PropertyResource(std::string name, GetterResource *getter) : NativePropertyResource{name}, _glue{nullptr}, _getter{getter}, _setter{nullptr} {}
    
    // native setter
    PropertyResource(std::string name, SetterResource *setter) : NativePropertyResource{name}, _glue{nullptr}, _getter{nullptr}, _setter{setter} {}
     /*/
public:
	virtual ~PropertyResource() {}
	virtual property_glue *glue() const override { return _glue; }
	virtual NativeFunctionResource *getter() const override { return _getter; }
    virtual void getter(NativeFunctionResource *getter) override { _getter = getter; }
	virtual NativeFunctionResource *setter() const override { return _setter; }
    virtual void setter(NativeFunctionResource *setter) override { _setter = setter; }
};

} // namespace hydro

#endif /* __h3o_PropertyResource_hpp */
