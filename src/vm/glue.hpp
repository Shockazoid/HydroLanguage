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

#ifndef __h3o_glue__
#define __h3o_glue__

#include <list>
#include <map>

#include "Closure.hpp"
#include "HvmContext.hpp"
#include "VM.hpp"
#include "EventContext.hpp"

namespace hydro
{

struct property_glue
{
	virtual ~property_glue() {}
	virtual bool get(HvmContext *threadContext, RuntimeContext *callee, VM *vm, HObject *instance, hvalue &value) = 0;
	virtual bool set(HvmContext *threadContext, RuntimeContext *callee, VM *vm, HObject *instance, hvalue &value) = 0;
};

struct hydro_property final : public property_glue
{
    const VM_Property *data;
    hydro_property(const VM_Property *vproperty) : data{vproperty} {}
	virtual ~hydro_property() {}
	virtual bool get(HvmContext *threadContext, RuntimeContext *callee, VM *vm, HObject *instance, hvalue &value) override
	{
        if(!instance || data->getter || data->setter)
        {
            // illegal!
            return false;
        }
        
        auto fields = instance->_cxt->fields;
        uint32_t n = instance->_cxt->_size;
        
        for(uint32_t i = 0; i < n; i++)
        {
            if(fields[i]->data == data)
            {
                value = fields[i]->value;
                return true;
            }
        }
        
        // fail
        return false;
	}
	virtual bool set(HvmContext *threadContext, RuntimeContext *callee, VM *vm, HObject *instance, hvalue &value) override
	{
		if(!instance || data->getter || data->setter)
        {
            // illegal!
            return false;
        }
        
        auto fields = instance->_cxt->fields;
        uint32_t n = instance->_cxt->_size;
        
        for(uint32_t i = 0; i < n; i++)
        {
            assert(fields[i]);
            if(fields[i]->data == data)
            {
                fields[i]->value = value;
                return true;
            }
        }
        
        // fail
        return false;
	}
};
/*
struct hydro_auto_getter final : public property_glue
{
    const VM_Property *data;
    hydro_auto_getter(const VM_Property *vproperty) : data{vproperty} {}
    virtual ~hydro_property() {}
    virtual bool get(HvmContext *threadContext, RuntimeContext *callee, VM *vm, Object *instance, hvalue &value) override
    {
        if(!instance || data->getter || data->setter)
        {
            // illegal!
            return false;
        }
        
        auto fields = instance->_cxt->fields;
        uint32_t n = instance->_cxt->_size;
        
        for(uint32_t i = 0; i < n; i++)
        {
            if(fields[i]->data == data)
            {
                value = fields[i]->value;
                return true;
            }
        }
        
        // fail
        return false;
    }
    virtual bool set(HvmContext *threadContext, RuntimeContext *callee, VM *vm, Object *instance, hvalue &value) override
    {
        // invalid
        return false;
    }
};

struct hydro_auto_setter final : public property_glue
{
    const VM_Property *data;
    hydro_auto_setter(const VM_Property *vproperty) : data{vproperty} {}
    virtual ~hydro_auto_setter() {}
    virtual bool get(HvmContext *threadContext, RuntimeContext *callee, VM *vm, Object *instance, hvalue &value) override
    {
        // invalid
        return false;
    }
    virtual bool set(HvmContext *threadContext, RuntimeContext *callee, VM *vm, Object *instance, hvalue &value) override
    {
        if(!instance || data->getter || data->setter)
        {
            // illegal!
            return false;
        }
        
        auto fields = instance->_cxt->fields;
        uint32_t n = instance->_cxt->_size;
        
        for(uint32_t i = 0; i < n; i++)
        {
            assert(fields[i]);
            if(fields[i]->data == data)
            {
                fields[i]->value = value;
                return true;
            }
        }
        
        // fail
        return false;
    }
};
*/
template <typename PropertyType, typename ClassType>
struct native_property : public property_glue
{
	PropertyType(ClassType::*ptr);
	native_property(PropertyType(ClassType::*prop)) : ptr{prop} {}
	virtual ~native_property() {}
	virtual bool get(HvmContext *threadContext, RuntimeContext *callee, VM *vm, HObject *instance, hvalue &value) override
	{
        if(ClassType *cast = dynamic_cast<ClassType *>(instance))
        {
            value = (cast->*ptr);
            return true;
        }
        
		return false;
	}
	virtual bool set(HvmContext *threadContext, RuntimeContext *callee, VM *vm, HObject *instance, hvalue &value) override
	{
        if(ClassType *cast = dynamic_cast<ClassType *>(instance))
        {
            (cast->*ptr) = value;
            return true;
        }
        
		return false;
	}
};

struct function_glue
{
	Closure *closure;
    function_glue(Closure *clos = nullptr) : closure{clos} {}
	virtual ~function_glue() {}
	virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::list<hvalue> &args, hvalue thisObject) = 0;
    virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject) = 0;
};

struct hydro_function : public function_glue
{
    hydro_function(Closure *clos = nullptr) : function_glue{clos} {}
	virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::list<hvalue> &args, hvalue thisObject) override
	{
		hvalue result;

		// invoke
		if (vm->invokeHydroFunc(threadContext, callee, closure, args, result, thisObject))
			return result;

		// void or error occurred
		return undefined;
	}
    virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject) override
    {
        hvalue result;

        // invoke
        if (vm->invokeHydroFunc(threadContext, callee, closure, args, result, thisObject))
            return result;

        // void or error occurred
        return undefined;
    }
};

template <typename ReturnType, typename... Arg>
struct native_function final : public function_glue
{
	ReturnType (*ptr)(Arg...);
    native_function(Closure *clos, ReturnType (*fn)(Arg...)) : function_glue{clos}, ptr{fn} {}
	native_function(ReturnType (*fn)(Arg...)) : ptr{fn} {}
	virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::list<hvalue> &args, hvalue thisObject) override
	{
		hvalue result;

		// invoke
		if (vm->invokeNativeFunc(threadContext, callee, ptr, closure, args, result, thisObject))
			return result;

		// void or error occurred
		return undefined;
	}
    virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject) override
    {
        hvalue result;

        // invoke
        if (vm->invokeNativeFunc(threadContext, callee, ptr, closure, args, result, thisObject))
            return result;

        // void or error occurred
        return undefined;
    }
};

struct hydro_method final : public function_glue
{
    hydro_method(Closure *clos) : function_glue{clos} {}
    hydro_method() : function_glue{} {}
    virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::list<hvalue> &args, hvalue thisObject) override
    {
        hvalue result;

        // invoke
        if (vm->invokeHydroFunc(threadContext, callee, closure, args, result, thisObject))
            return result;

        // void or error occurred
        return undefined;
    }
    virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject) override
    {
        hvalue result;

        // invoke
        if (vm->invokeHydroFunc(threadContext, callee, closure, args, result, thisObject))
            return result;

        // void or error occurred
        return undefined;
    }
};

template <typename ReturnType, typename ClassType, typename... Arg>
struct native_method final : public function_glue
{
	ReturnType (ClassType::*ptr)(Arg...);
    native_method(Closure *clos, ReturnType (ClassType::*mth)(Arg...)) : function_glue{clos}, ptr{mth} {}
    native_method(ReturnType (ClassType::*mth)(Arg...)) : function_glue{}, ptr{mth} {}
	virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::list<hvalue> &args, hvalue thisObject) override
	{
		hvalue result;

		// invoke
		if (vm->invokeNativeMethod(threadContext, callee, ptr, closure, args, result, thisObject))
			return result;

		// void or error occurred
		return undefined;
	}
    virtual hvalue call(HvmContext *threadContext, RuntimeContext *callee, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject) override
    {
        hvalue result;

        // invoke
        if (vm->invokeNativeMethod(threadContext, callee, ptr, closure, args, result, thisObject))
            return result;

        // void or error occurred
        return undefined;
    }
};

struct action_glue
{
    Closure *closure;
    action_glue(Closure *clos = nullptr) : closure{clos} {}
    virtual ~action_glue() {}
    virtual hvalue trigger(HvmContext *threadContext, class HAction *callee, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject) = 0;
    virtual hvalue trigger(HvmContext *threadContext, class HAction *callee, VM *vm, EventContext *eventContext, hvalue thisObject) = 0;
};

struct hydro_action final : public action_glue
{
    hydro_action(Closure *clos = nullptr) : action_glue{clos} {}
    virtual ~hydro_action() {}
    virtual hvalue trigger(HvmContext *threadContext, class HAction *callee, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject) override
    {
        hvalue result;
        
        if (vm->triggerHydroAction(threadContext, callee, closure, args, result, thisObject))
        {
            // return value
            return result;
        }
        
        // returned void
        return undefined;
    }
    virtual hvalue trigger(HvmContext *threadContext, class HAction *callee, VM *vm, EventContext *eventContext, hvalue thisObject) override
    {
        hvalue result;
        /*
        if (vm->triggerHydroAction(threadContext, callee, closure, eventContext, result, thisObject))
        {
            // returned value
            return result;
        }
        */
        // returned void
        return undefined;
    }
};
/*
template <typename ReturnType, typename ...Arg>
struct native_action final : public action_glue
{
    ReturnType (*ptr)(Arg...);
    native_action(ReturnType (*act)(Arg...)) : action_glue{}, ptr{act} {}
    virtual ~native_action() {}
    virtual bool trigger(HvmContext *threadContext, RuntimeContext *callee, VM *vm, void *eventContext, hvalue thisObject)
    {
        return vm->triggerNativeAction(threadContext, action, vm, eventContext, thisObject);
    }
};

template <typename ReturnType, typename ClassType, typename ...Arg>
struct native_member_action final : public action_glue
{

    ReturnType (ClassType::*ptr)(Arg...);
    native_member_action(ReturnType (*act)(Arg...)) : action_glue{}, ptr{act}
    virtual ~native_member_action() {}
    virtual bool trigger(HvmContext *threadContext, RuntimeContext *callee, VM *vm, void *eventContext, hvalue thisObject)
    {
        return vm->triggerNativeAction(threadContext, callee, ptr, closure, eventContext, thisObject));
    }
};
*/
} // namespace hydro

#endif /* __h3o_function_glue__ */
