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

#ifndef __h3o_ClassResource__
#define __h3o_ClassResource__

#include <vector>

#include "NativeResource.hpp"
#include "ConstructorResource.hpp"
#include "PropertyResource.hpp"
#include "MethodResource.hpp"
#include "../ClassAllocator.hpp"
#include "../../memory/object_ptr.hpp"

namespace hydro
{

class HvmEnv;
struct VM_Class;

template <typename ClassType>
class ClassResource : public NativeClassResource
{
    friend class NativeResourceRegistry;
	friend struct NativeService;

private:
	ConstructorResource<ClassType> *_constructor;
	std::vector<NativePropertyResource *> _properties;
	std::vector<NativeFunctionResource *> _methods;
    ClassAllocator *_allocator;
    
    ClassResource(std::string qualifiedClassName, ClassAllocator *allocator = nullptr) : NativeClassResource{qualifiedClassName}, _constructor{nullptr}, _properties{}, _methods{}, _allocator{allocator}
    {
    }
    
    template <typename... Arg>
    ClassResource(std::string qualifiedClassName, void (*constructor)(Arg...)) : NativeClassResource{qualifiedClassName}, _constructor{new ConstructorResource<ClassType>(constructor)}, _properties{}, _methods{}, _allocator{nullptr}
    {
        if (typeid(ClassType) != typeid(void))
            _allocator = new NativeClassAllocator<ClassType>{};
    }
    
    template <typename... Arg>
    ClassResource(std::string qualifiedClassName, void (ClassType::*constructor)(Arg...)) : NativeClassResource{qualifiedClassName}, _constructor{new ConstructorResource<ClassType>(constructor)}, _properties{}, _methods{}, _allocator{nullptr}
    {
        if (typeid(ClassType) != typeid(void))
            _allocator = new NativeClassAllocator<ClassType>{};
    }
    
	template <typename PropertyType>
	void appendProperty(std::string name, PropertyType(ClassType::*prop))
	{
        NativePropertyResource *property = lookupProperty(name);
        
		if (property)
		{
			if (property->getter() || property->setter())
			{
				// already bound
				throw std::runtime_error{"Native property binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
			}
		}
		else if (exists(name))
			throw std::runtime_error{"Native binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};

        _properties.push_back(new PropertyResource{name, prop});
	}

	template <typename PropertyType, typename... Arg>
	void appendPropertyGetter(std::string name, PropertyType (*getter)(Arg...))
	{
		NativePropertyResource *property = lookupProperty(name);

        if (property)
        {
            if (property->getter() || property->glue())
            {
                // already bound
                throw std::runtime_error{"Native property binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
            }
        }
        else if (exists(name))
            throw std::runtime_error{"Native binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
        
        if(!property)
        {
            property = new PropertyResource{name};
            _properties.push_back(property);
        }
        
        //if(property->type() != typeid(PropertyType))
        //    throw std::runtime_error{""}; // TODO describe error
        
        auto get = new GetterResource{getter};
        property->getter(get);
	}

	template <typename PropertyType, typename... Arg>
	void appendPropertyGetter(std::string name, PropertyType (ClassType::*getter)(Arg...))
	{
		NativePropertyResource *property = lookupProperty(name);

		if (property)
		{
			if (property->getter() || property->glue())
			{
				// already bound
				throw std::runtime_error{"Native property binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
			}
		}
		else if (exists(name))
			throw std::runtime_error{"Native binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
        
        if(!property)
        {
            property = new PropertyResource{name};
            _properties.push_back(property);
        }
        
        //if(property->type() != typeid(PropertyType))
        //    throw std::runtime_error{""}; // TODO describe error
        
        auto get = new GetterResource{getter};
        property->getter(get);
	}

	template <typename PropertyType, typename... Arg>
	void appendPropertySetter(std::string name, void (*setter)(PropertyType, Arg...))
	{
        NativePropertyResource *property = lookupProperty(name);

        if (property)
        {
            if (property->setter() || property->glue())
            {
                // already bound
                throw std::runtime_error{"Native property binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
            }
        }
        else if (exists(name))
            throw std::runtime_error{"Native binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
        
        if(!property)
        {
            property = new PropertyResource{name};
            _properties.push_back(property);
        }
        
        //if(property->type() != typeid(PropertyType))
        //    throw std::runtime_error{""}; // TODO describe error
        
        auto set = new SetterResource{setter};
        property->setter(set);
	}

	template <typename... Arg>
	void appendPropertySetter(std::string name, void (ClassType::*setter)(Arg...))
	{
		NativePropertyResource *property = lookupProperty(name);

        if (property)
        {
            if (property->setter() || property->glue())
            {
                // already bound
                throw std::runtime_error{"Native property binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
            }
        }
        else if (exists(name))
            throw std::runtime_error{"Native binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};
        
        if(!property)
        {
            property = new PropertyResource{name};
            _properties.push_back(property);
        }
        
        //if(property->type() != typeid(PropertyType))
        //    throw std::runtime_error{""}; // TODO describe error
        
        auto set = new SetterResource{setter};
        property->setter(set);
	}

	template <typename ReturnType, typename... Arg>
	void appendMethod(std::string name, ReturnType (ClassType::*method)(Arg...))
	{
		if (exists(name))
			throw std::runtime_error("Native binding '" + name + "' is already defined on native type '" + qualifiedName() + "'.");

		_methods.push_back(new MethodResource<ClassType>(name, method));
	}

public:
	virtual ~ClassResource() {}
    
    
	// property (member pointer)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType(ClassType::*property))
	{
		appendProperty(name, property);
		return this;
	}
    
	// getter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (*getter)(hvalue))
	{
		appendPropertyGetter(name, getter);
		return this;
	}

	// getter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (*getter)(hobject))
	{
		appendPropertyGetter(name, getter);
		return this;
	}

	// getter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (*getter)(HvmEnv *, hvalue))
	{
		appendPropertyGetter(name, getter);
		return this;
	}

	// getter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (*getter)(HvmEnv *, hobject))
	{
		appendPropertyGetter(name, getter);
		return this;
	}

	// getter
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (ClassType::*getter)())
	{
		appendPropertyGetter(name, getter);
		return this;
	}

	// getter
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (ClassType::*getter)(HvmEnv *))
	{
		appendPropertyGetter(name, getter);
		return this;
	}

	//  setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, void (*setter)(hvalue, MemberType))
	{
		appendPropertySetter(name, setter);
		return this;
	}

	//  setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, void (*setter)(hobject, MemberType))
	{
		appendPropertySetter(name, setter);
		return this;
	}

	//  setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, void (*setter)(HvmEnv *, hvalue, MemberType))
	{
		appendPropertySetter(name, setter);
		return this;
	}

	//  setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, void (*setter)(HvmEnv *, hobject, MemberType))
	{
		appendPropertySetter(name, setter);
		return this;
	}

	//  setter (method)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, void (ClassType::*setter)(MemberType))
	{
		appendPropertySetter(name, setter);
		return this;
	}

	//  setter (method)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, void (ClassType::*setter)(HvmEnv *, MemberType))
	{
		appendPropertySetter(name, setter);
		return this;
	}

	//  getter/setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (*getter)(), void (*setter)(MemberType))
	{
		appendPropertyGetter(name, getter);
		appendPropertySetter(name, setter);
		return this;
	}

	//  getter/setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (*getter)(hvalue, HvmEnv *), void (*setter)(HvmEnv *, hobject, MemberType))
	{
		appendPropertyGetter(name, getter);
		appendPropertySetter(name, setter);
		return this;
	}

	//  getter/setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (*getter)(hobject, HvmEnv *), void (*setter)(HvmEnv *, hobject, MemberType))
	{
		appendPropertyGetter(name, getter);
		appendPropertySetter(name, setter);
		return this;
	}

	//  getter/setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (ClassType::*getter)(), void (ClassType::*setter)(MemberType))
	{
		appendPropertyGetter(name, getter);
		appendPropertySetter(name, setter);
		return this;
	}

	//  getter/setter (function)
	template <typename MemberType>
	ClassResource<ClassType> *property(std::string name, MemberType (ClassType::*getter)(HvmEnv *), void (ClassType::*setter)(HvmEnv *, MemberType))
	{
		appendPropertyGetter(name, getter);
		appendPropertySetter(name, setter);
		return this;
	}
    
	// method
	template <typename ReturnType, typename... Arg>
	ClassResource<ClassType> *method(std::string name, ReturnType (ClassType::*method)(HvmEnv *env, Arg...))
	{
		if (exists(name))
			throw std::runtime_error{"Native binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};

		_methods.push_back(new MethodResource<ClassType>(name, method));
		return this;
	}

    template <typename ReturnType, typename... Arg>
	ClassResource<ClassType> *method(std::string name, ReturnType (ClassType::*method)(Arg...))
	{
		if (exists(name))
            throw std::runtime_error{"Native binding '" + name + "' is already defined on native type '" + qualifiedName() + "'."};

        _methods.push_back(new MethodResource<ClassType>(name, method));
		return this;
	}
    
	template <typename ReturnType, typename... Arg>
	ClassResource<ClassType> *method(std::string name, ReturnType (*method)(hvalue, Arg...))
	{
		appendMethod(name, method);
		return this;
	}

	template <typename ReturnType, typename... Arg>
	ClassResource<ClassType> *method(std::string name, ReturnType (*method)(hobject, Arg...))
	{
		appendMethod(name, method);
		return this;
	}

	template <typename ReturnType, typename... Arg>
	ClassResource<ClassType> *method(std::string name, ReturnType (*method)(HvmEnv *, hvalue, Arg...))
	{
		appendMethod(name, method);
		return this;
	}

	template <typename ReturnType, typename... Arg>
	ClassResource<ClassType> *method(std::string name, ReturnType (*method)(HvmEnv *, hobject, Arg...))
	{
		appendMethod(name, method);
		return this;
	}

	template <typename ReturnType, typename InstanceType, typename... Arg>
	ClassResource<ClassType> *method(std::string name, ReturnType (*method)(HvmEnv *, object_ptr<InstanceType>, Arg...))
	{
		appendMethod(name, method);
		return this;
	}
    
	bool exists(std::string name)
	{
		return lookupProperty(name) != nullptr || lookupMethod(name) != nullptr;
	}
    
    virtual NativePropertyResource *lookupProperty(std::string name) override
    {
        for (NativePropertyResource *prop : _properties)
            if (prop->name() == name)
                return prop;
        
        // fail
        return nullptr;
    }
    
    virtual NativeFunctionResource *lookupMethod(std::string name) override
    {
        for (NativeFunctionResource *meth : _methods)
            if (meth->name() == name)
                return meth;
        
        // fail
        return nullptr;
    }
    
	virtual const std::type_info &type() const override { return typeid(ClassType); }
    virtual ClassAllocator * allocator() const override { return _allocator; }
    virtual NativeFunctionResource *constructor() const override { return _constructor; }
};

} // namespace hydro

#endif /* __h3o_ClassResource__ */
