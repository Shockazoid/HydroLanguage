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

#include "HObjectContext.hpp"
#include "HMethod.hpp"
#include "HProperty.hpp"
#include "HGetter.hpp"
#include "HSetter.hpp"
#include "HClass.hpp"
#include "HEventType.hpp"
#include "HEventContext.hpp"
#include "HAction.hpp"
#include "../vm/HvmEnv.hpp"
#include "../vm/HvmContext.hpp"

namespace hydro
{

HObjectContext::HObjectContext(HObject *instance, HClass *rclass) : _instance{instance}, _rclass{rclass}, _rstatic{nullptr}
{
    setup();
}

HObjectContext::HObjectContext(HObject *instance, HClass *rclass, HClass *rstatic) : _instance{instance}, _rclass{rclass}, _rstatic{rstatic}
{
    setup();
}

HObjectContext::~HObjectContext()
{
	for (uint16_t i = 0; i < _capacity; i++)
		delete fields[i];

	delete[] fields;
}

void HObjectContext::setup()
{
    // initialize fields
    _size = 0;
    
    std::stack<HClass *> classTree; // class hierarchy
    HClass *c = _rclass;
    const ClassData *vclass;
    
    while (c)
    {
        classTree.push(c);
        vclass = c->vclass();
        _size += vclass->ninstance;
        c = c->superClass(); // next
    }
    
    if(_rstatic)
    {
        // allocate memory for static members (this is a class context)
        _size += _rstatic->_vclass->nstatic;
    }
    
    _capacity = _size;
    
    // allocate memory for properties
    fields = new property_space *[_size];
    property_space *newField;
    int32_t i = 0, j;
    
    // starting with base class, initialize all fields
    while (!classTree.empty())
    {
        c = classTree.top();
        classTree.pop();
        vclass = c->vclass();
        
        for(j = 0; j < vclass->nfields; j++)
        {
            VMObject *data = vclass->fields[i];
            
            // property?
            if(PropertyData *prop = dynamic_cast<PropertyData *>(data))
            {
                if(is_static(prop->modifier))
                    continue; // skip static properties
                
                // found
                newField = new property_space{};
                newField->data = prop;
                newField->dynamic = false;
                newField->key = prop->name;
                newField->value = undefined;
                fields[i++] = newField; // put
            }
        }
    }
    
    // for class context
    if(_rstatic)
    {
        // put static members in class instance
        for(j = 0; j < _rstatic->_vclass->nfields; j++)
        {
            if(!is_static(_rstatic->_vclass->fields[j]->modifier))
                continue; // skip
            
            if(PropertyData *prop = dynamic_cast<PropertyData *>(_rstatic->_vclass->fields[j]))
            {
                newField = new property_space{};
                newField->data = prop;
                newField->dynamic = false;
                newField->key = prop->name;
                newField->value = undefined;
                fields[i++] = newField; // put
            }
            else if(MethodData *meth = dynamic_cast<MethodData *>(_rstatic->_vclass->fields[j]))
            {
                newField = new property_space{};
                newField->data = meth;
                newField->dynamic = false;
                newField->key = meth->name;
                newField->value = _instance->_env->createFunction(meth);
                fields[i++] = newField; // put
            }
            else
            {
                // bad implementation
                throw std::runtime_error{"Received unexpected member type."};
            }
        }
    }
}

HProperty *HObjectContext::isProperty(RuntimeContext *member) { return dynamic_cast<HProperty *>(member); }

HMethod *HObjectContext::isMethod(RuntimeContext *member) { return dynamic_cast<HMethod *>(member); }

bool HObjectContext::checkAccess(RuntimeContext *callee, RuntimeContext *caller)
{
	if (!caller || callee == caller)
	{
		// caller is native code (all access pass)
		return true;
	}

    uint16_t mod = callee->modifier();

	if (is_public(mod))
	{
		// global access
		return true;
	}
	else if (is_internal(mod))
	{
		// package access
		std::string pkg = callee->ownerPackage();

		if (!pkg.empty())
		{
			if (pkg == caller->ownerPackage())
				return true;
		}
		else
			; // error! context marked as internal but does not belong to a package
	}
	else if (is_protected(mod))
	{
		// subclass access (hierarchical)
		// Rule: Access is allowed if and only if the class is the same or is a subclass.
		HClass *classA = callee->ownerClass();
		HClass *classB = caller->ownerClass();

		// find subclass
		while (classB)
		{
			if (classB == classA)
				return true; // success!

			// try again...
			classB = caller->ownerClass();
		}

		// not allowed
		return false;
	}
	else if (is_private(mod))
	{
		// class access
		// Rule: Access is allowed if and only if the class is the same.
		HClass *clss = callee->ownerClass();

		if (clss)
			return clss == caller->ownerClass();
		else
			; // error! context marked as private but does not belong to a class.
	}
	else if (is_restricted(mod))
	{
		// vm access
		// Rule: Access is allowed if and only if the caller is the virtual machine or another virtualized context.
		return is_restricted(caller->modifier());
	}
	else
	{
		// local (default)
		return true;
	}

	// error!
	return false;
}

void HObjectContext::ensureCapacity(uint16_t capacity)
{
	if (_capacity >= capacity)
		return; // do nothing

	uint16_t newCap = capacity;

	property_space **tmp = new property_space *[newCap];

	uint16_t i;

	// copy
	for (i = 0; i < _capacity; i++)
		tmp[i] = fields[i];

	for (; i < newCap; i++)
		tmp[i] = nullptr; // empty

	_capacity = newCap;
	delete[] fields;
	fields = tmp;
}

void HObjectContext::defineDynamicProperty(std::string name, const hvalue &value)
{
	property_space *field = new property_space{};
	field->data = nullptr;
	field->dynamic = true;
	field->key = name;
	field->value = value;
	ensureCapacity(_size + 1);
	fields[_size++] = field;
}


bool HObjectContext::isAncestorClass(HClass *classContext)
{
    HClass *c = _rclass->superClass();
    
    while(c)
    {
        if (c == classContext)
            return true; // success!
        c = c->superClass();
    }
    
    // not an ancestor
    return false;
}

bool HObjectContext::isSubClass(HClass *classContext)
{
    HClass *c = classContext->superClass();
    
    while (c)
    {
        if(c == classContext)
            return true; // success!
        c = c->superClass();
    }
    
    // not a subclass
    return true;
}

RuntimeContext *HObjectContext::lookup(std::string name, HClass *classContext)
{
    RuntimeContext *member;
    
    do
    {
        member = classContext->lookup(name);
        
        if (member)
            return member;
        
        classContext = classContext->superClass();
    }
    while (classContext);
    
    // fail
    return nullptr;
}

RuntimeContext *HObjectContext::tryResolve(VM *vm, HvmContext *threadContext, std::string name, RuntimeContext *callingContext)
{
    HClass *classContext = _rclass;
    RuntimeContext *member;
    
    while (classContext)
    {
        member = classContext->lookup(name);
        
        if (member)
            return member;
        
        classContext = classContext->superClass();
    }
    
    // fail
    return nullptr;
}

bool HObjectContext::getProperty(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, std::string name, hvalue &value)
{
    RuntimeContext *member = tryResolve(vm, threadContext, name, _rclass);
    
    if(member && is_static(member->modifier()))
    {
        // cannot access static member
        return false;
    }
    else if (!member)
    {
        if(_rstatic)
            member = lookup(name, _rstatic);
    }
    
    if(HProperty *prop = dynamic_cast<HProperty *>(member))
    {
        if (!checkAccess(prop, callingContext))
        throw std::runtime_error{"Attempted access of " + get_accessor(prop->modifier()) + " property \"" + name + "\" on object of type \"" + _rclass->assemblyName() + "\"."};
        
        return prop->get(threadContext, vm, _instance, value);
    }
    else if(HMethod *meth = dynamic_cast<HMethod *>(member))
    {
        if (!checkAccess(meth, callingContext))
            throw std::runtime_error{"Attempted access of " + get_accessor(meth->modifier()) + " method \"" + name + "\" on object of type \"" + _rclass->assemblyName() + "\"."};

        value = env->bindMethod(meth, _instance);
        return true;
    }
    
	// try dynamic
	if (is_dynamic(_rclass->modifier()))
	{
		for (uint16_t i = 0; i < _size; i++)
		{
			auto field = fields[i];
			if (field->key == name)
			{
				value = field->value;
				return true; // success!
			}
		}

		// property was not defined
		value = undefined;
		return true;
	}

	// try native
	if (_instance->tryGetProperty(env, vm, threadContext, name, value))
		return true; // success!

	// fail (causes runtime error)
	return false;
}

bool HObjectContext::setProperty(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, std::string name, const hvalue &value)
{
    RuntimeContext *member = tryResolve(vm, threadContext, name, _rclass);
    
    if(member && is_static(member->modifier()))
    {
        // cannot access static member
        return false;
    }
    else if (!member)
    {
        if(_rstatic)
            member = lookup(name, _rstatic);
    }
    
    if (HProperty *prop = isProperty(member))
    {
        if (!checkAccess(prop, callingContext))
            throw std::runtime_error{"Attempted access of " + get_accessor(prop->modifier()) + " method \"" + name + "\" on object of type \"" + _rclass->assemblyName() + "\"."};
        return prop->set(threadContext, vm, _instance, value);
    }
    else if (HMethod *meth = isMethod(member))
    {
        if (!checkAccess(meth, callingContext))
            throw std::runtime_error{"Attempted invocation of " + get_accessor(meth->modifier()) + " method \"" + name + "\" on object of type \"" + _rclass->assemblyName() + "\"."};
        throw std::runtime_error{"Cannot set method constant \"" + name + "\"."};
    }

	// try dynamic
	if (is_dynamic(_rclass->modifier()))
	{
		for (uint16_t i = 0; i < _size; i++)
		{
			auto field = fields[i];
			if (field->key == name)
			{
				field->value = value;
				return true; // success!
			}
		}

		// create non-existent property
		defineDynamicProperty(name, value);
		return true;
	}

	// try native
	if (_instance->trySetProperty(env, vm, threadContext, name, value))
		return true; // success!

	// fail (causes runtime error)
	return false;
}

bool HObjectContext::getIndex(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, const hvalue &index, hvalue &value)
{
	// try native only
	if (_instance->tryGetIndex(env, vm, threadContext, index, value))
		return true; // success!

	if (is_string(index) && !is_dynamic(_rclass->modifier()))
	{
		std::string key = index;

		for (uint16_t i = 0; i < _size; i++)
		{
			auto field = fields[i];
			if (field->key == key)
			{
				field->value = value;
				return true; // success!
			}
		}
	}

	// fail (causes runtime error)
	return false;
}

bool HObjectContext::setIndex(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, const hvalue &key, const hvalue &value)
{
	// try native only
	if (_instance->trySetIndex(env, vm, threadContext, key, value))
		return true; // success!

	// fail (causes runtime error)
	return false;
}

bool HObjectContext::callProperty(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, std::string name, std::list<hvalue> &args, hvalue &result)
{
    RuntimeContext *member = tryResolve(vm, threadContext, name, _rclass);
    
    if(member && is_static(member->modifier()))
    {
        // cannot access static member
        return false;
    }
    else if (!member)
    {
        if(_rstatic)
            member = lookup(name, _rstatic);
    }
    
	if (HMethod *meth = isMethod(member))
	{
		if (!checkAccess(meth, callingContext))
			throw std::runtime_error{"Attempted invocation of " + get_accessor(meth->modifier()) + " method \"" + name + "\" on object of type \"" + _rclass->assemblyName() + "."};
		result = meth->call(env, vm, threadContext, args, _instance);
		return true; // success!
	}
    else if (HProperty *prop = isProperty(member))
	{
		hvalue callable;
		if (!checkAccess(meth, callingContext))
			throw std::runtime_error{"Attempted invocation of " + get_accessor(meth->modifier()) + " method \"" + name + "\" on object of type \"" + _rclass->assemblyName() + "\"."};

		if (prop->get(threadContext, vm, _instance, callable))
		{
			if (!env->call(callable, args, result))
				result = undefined;
			return true; // success!
		}
	}

	// try dynamic
	if (is_dynamic(_rclass->modifier()))
	{
		for (uint16_t i = 0; i < _size; i++)
		{
			property_space *field = fields[i];
			hvalue value = field->value;

			if (field->key == name)
			{
				if (is_callable(value))
				{
					env->call(value, args, result); // result is guaranteed by vm
					return true;			  // success!
				}
			}
		}
	}

	// try native
	if (_instance->tryCallProperty(env, vm, threadContext, name, args, result))
		return true; // success!

	// fail (causes runtime error)
	return false;
}

bool HObjectContext::callIndex(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, const hvalue &key, std::list<hvalue> &args, hvalue &result)
{
	// try native only
	if (_instance->tryCallIndex(env, vm, threadContext, key, args, result))
		return true; // success!

	// fail (causes runtime error)
	return false;
}

bool HObjectContext::deleteProperty(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, std::string name)
{

	// fail (no runtime error)
	return false;
}

bool HObjectContext::deleteIndex(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, const hvalue &key)
{

	// fail (no runtime error)
	return false;
}

void HObjectContext::copyToMap(std::map<std::string, hvalue> &map) const
{
    for(uint16_t i = 0; i < _size; i++)
        if(fields[i]->dynamic)
            map.insert(std::pair<std::string, hvalue>(fields[i]->key, fields[i]->value));
}

} // namespace hydro
