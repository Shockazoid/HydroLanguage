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

#ifndef __h3o_VM__
#define __h3o_VM__

#include <list>
#include <cstdlib>
#include <stack>
#include <map>

#include "CallFrame.hpp"
#include "FuncData.hpp"
#include "VM_Constructor.hpp"
#include "PropertyData.hpp"
#include "MethodData.hpp"
#include "opcodes.hpp"
#include "modifiers.hpp"
#include "HvmContext.hpp"
#include "defaults.hpp"
#include "hvm_ref_cast.hpp"
#include "hvm_auto_wire.hpp"
#include "NilTypeSpecifier.hpp"
#include "../system/HObject.hpp"
#include "../system/HObjectContext.hpp"
#include "../system/HClass.hpp"
#include "../system/HList.hpp"
#include "../system/HDictionary.hpp"
#include "../system/HString.hpp"
#include "../utility/classhelper.hpp"

namespace hydro
{

class VM
{

	friend class HvmEnv;

private:
    struct hvm_abort{};
    
    struct hvm_halt{};
    
	class HvmEnv *_env;
	uint32_t _activeThreads;
	MemorySpace *_globals;
	bool _running;
	std::stack<hvalue> _opndStack;

	template <typename... Arg>
	void callVoidFunc(void (*func)(Arg...), std::list<hvalue> &args, int32_t nargs)
	{
		if (sizeof...(Arg) != args.size())
			throw std::runtime_error("Native function encountered argument count mismatch! Received " + std::to_string(args.size()) + " but expected " + std::to_string(sizeof...(Arg)) + "."); // todo argument mismatch exception

		args.reverse();
		func(hvm_ref_cast<Arg>().do_cast(fetch_back(args))...);
	}

	template <typename ReturnType, typename... Arg>
	ReturnType callReturnFunc(ReturnType (*func)(Arg...), std::list<hvalue> &args, int32_t nargs)
	{
		if (sizeof...(Arg) != args.size())
            throw std::runtime_error("Native function encountered argument count mismatch! Received " + std::to_string(args.size()) + " but expected " + std::to_string(sizeof...(Arg)) + "."); // todo argument mismatch exception

		args.reverse();
		return func(hvm_ref_cast<Arg>().do_cast(fetch_back(args))...);
	}

	template <typename ClassType, typename... Arg>
	static hvalue callVoidMethod(ClassType *instance, void (ClassType::*method)(Arg...), std::list<hvalue> &args, int32_t nargs)
	{
		if (sizeof...(Arg) != args.size())
            throw std::runtime_error("Native function encountered argument count mismatch! Received " + std::to_string(args.size()) + " but expected " + std::to_string(sizeof...(Arg)) + "."); // todo argument mismatch exception

		args.reverse();

		(instance->*method)(hvm_ref_cast<Arg>().do_cast(fetch_back(args))...);
		return undefined;
	}

	template <typename ReturnType, typename ClassType, typename... Arg>
	static hvalue callReturnMethod(ClassType *instance, ReturnType (ClassType::*method)(Arg...), std::list<hvalue> &args, int32_t nargs)
	{
		if (sizeof...(Arg) != args.size())
            throw std::runtime_error("Native function encountered argument count mismatch! Received " + std::to_string(args.size()) + " but expected " + std::to_string(sizeof...(Arg)) + "."); // todo argument mismatch exception

		args.reverse();
        return (instance->*method)(hvm_ref_cast<Arg>().do_cast(fetch_back(args))...);
	}
    
	std::string describe(hvalue val);

	void opush(CallFrame *frame, hvalue opnd);
	hvalue opop(CallFrame *frame);
	hvalue opeek(CallFrame *frame);
	CallFrame *currentFrame(HvmContext *cxt);
	MemorySpace *findVar(std::string name, MemorySpace *scope);
	MemorySpace *findVarWithDynamicScaope(std::string name, CallFrame *frame);
	bool cpu(HvmContext *cxt, Chunk *chunk, CallFrame *currentFrame, hvalue &result);
	void call(HvmContext *threadContext, hvalue callable, std::list<hvalue> &args, hvalue &result, hvalue thisObject = nullptr);
    void call(HvmContext *threadContext, hvalue callable, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject = nullptr);
    void trigger(HvmContext *cxt, hvalue triggerable, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject = nullptr);

	CallFrame *prepVmCall(HvmContext *threadContext, RuntimeContext *callee, Closure *closure, std::list<hvalue> &args, MemorySpace *scopeToUse = nullptr, hvalue thisObject = nullptr, bool useDynamicScope = false);
    CallFrame *prepVmCall(HvmContext *threadContext, RuntimeContext *callee, Closure *closure, std::map<std::string, hvalue> &map, std::list<hvalue> &args, MemorySpace *scopeToUse = nullptr, hvalue thisObject = nullptr, bool useDynamicScope = false);
    CallFrame *prepVmTrigger(HvmContext *threadContext, RuntimeContext *callee, Closure *closure, std::map<std::string, hvalue> &args, MemorySpace *scopeToUse = nullptr, hvalue thisObject = nullptr, bool useDynamicScope = false);

	void dissasemble(uint8_t opcode, uint32_t ip, Chunk *chunk);
	void dumpMemory(HvmContext *cxt);

	HvmContext *newHeap();

	template <typename ReturnType, typename... Arg>
	bool callFunction(ReturnType (*fn)(Arg...), std::list<hvalue> &args, hvalue &result, int32_t nargs)
	{
		result = callReturnFunc(fn, args, nargs);
		return true; // always returns a value
	}

	template <typename... Arg>
	bool callFunction(void (*fn)(Arg...), std::list<hvalue> &args, hvalue &result, int32_t nargs)
	{
		callVoidFunc(fn, args, nargs);
		return false; // never returns a value
	}

	template <typename ReturnType, typename... Arg>
	bool callMethod(ReturnType (*fn)(Arg...), std::list<hvalue> &args, hvalue &result, int32_t nargs)
	{
		result = call_func_return(fn, args, nargs);
		return true; // always returns a value
	}

	template <typename ClassType, typename... Arg>
	bool callMethod(void (ClassType::*fn)(Arg...), std::list<hvalue> &args, hvalue &result, int32_t nargs)
	{
		callVoidFunc(fn, args, nargs);
		return false; // never returns a value
	}

	template <typename ReturnType, typename ClassType, typename... Arg>
	bool callMethod(hvalue instance, ReturnType (ClassType::*fn)(Arg...), std::list<hvalue> &args, hvalue &result, int32_t nargs)
	{
		ClassType *o = instance.tryCast<ClassType>();
		result = callReturnMethod((ClassType *)o, fn, args, nargs);
		return true; // always returns a value
	}

	template <typename ClassType, typename... Arg>
	bool callMethod(hvalue instance, void (ClassType::*fn)(Arg...), std::list<hvalue> &args, hvalue &result, int32_t nargs)
	{
		ClassType *o = instance.tryCast<ClassType>();
		result = callVoidMethod((ClassType *)o, fn, args, nargs);
		return false; // never returns a value
	}
        
    hvalue invokeConstructor(HvmContext *threadContext, hobject instance, std::list<hvalue> &args);

public:
    struct hvm_uncaught_error final
    {
        friend class VM;
        
    public:
        hvalue target;
        ~hvm_uncaught_error() {}
        
    private:
        hvm_uncaught_error(hvalue t) : target{t} {}
    };
    
	VM(HvmEnv *env, MemorySpace *globals);
	~VM();

	bool exec(HvmContext *cxt, Chunk *prog, hvalue &result, MemorySpace *scopeToUse);

    hvalue newInstance(HvmContext *cxt, HClass *type, std::list<hvalue> &args);
    
    template <typename ...Arg>
    hvalue newInstanceWithArgs(HvmContext *cxt, HClass *type, Arg...args);
    
	bool invokeHydroFunc(HvmContext *threadContext, RuntimeContext *callingContext, Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject = nullptr);
    
    bool invokeHydroFunc(HvmContext *threadContext, RuntimeContext *callingContext, Closure *closure, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject = nullptr);

    template <typename ReturnType, typename ...Arg>
    void inject(HvmContext *cxt, std::list<hvalue> &args, uint16_t &nwired, ReturnType (*fn)(Arg...))
    {
        // inject dependencies into argument list for native functions that represent hydro functions
        AutoWireContext autoWire = AutoWireContext{};
        autoWire.env = _env;
        autoWire.vm = this;
        autoWire.cxt = cxt;
        int _[] = {0, (hvm_auto_wire<Arg>{}.put(args, autoWire, nwired), 0)...};
        (void)_;
    }
    
    template <typename ReturnType, typename ...Arg>
    void inject(HvmContext *cxt, hvalue instance, std::list<hvalue> &args, uint16_t &nwired, ReturnType (*fn)(Arg...))
    {
        // inject dependencies into argument list for native functions that represent hydro methods
        AutoWireContext autoWire = AutoWireContext{};
        autoWire.env = _env;
        autoWire.vm = this;
        autoWire.cxt = cxt;
        int _[] = {0, (hvm_auto_wire<Arg>{}.put(instance, args, autoWire, nwired), 0)...};
        (void)_;
    }
    
    template <typename ReturnType, typename ClassType, typename ...Arg>
    void inject(HvmContext *cxt, std::list<hvalue> &args, uint16_t &nwired, ReturnType (ClassType::*fn)(Arg...))
    {
        // inject dependencies into argument list for native methods that represent hydro methods
        AutoWireContext autoWire = AutoWireContext{};
        autoWire.env = _env;
        autoWire.vm = this;
        autoWire.cxt = cxt;
        int _[] = {0, (hvm_auto_wire<Arg>{}.put(args, autoWire, nwired), 0)...};
        (void)_;
    }
    
	template <typename ReturnType, typename... Arg>
	bool invokeNativeFunc(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (*fn)(Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
	{
		prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);
        
        // argument count
        uint16_t nargs = (uint16_t)args.size();
        
        // number of arguments that are injected dependencies (i.e., not defined in Hydro)
        uint16_t nwired = 0;
        
        // dependency injection
        inject(threadContext, args, nwired, fn);
        
		// call non-void function
		return callFunction(fn, args, result, nargs - nwired);
	}
    
    template <typename ReturnType, typename... Arg>
    bool invokeNativeFunc(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (*fn)(Arg...), Closure *closure, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject)
    {
        // prep
        std::list<hvalue> argList;
        prepVmCall(threadContext, callingContext, closure, args, argList, nullptr, thisObject);
        
        // argument count
        uint16_t nargs = (uint16_t)args.size();
        
        // number of arguments that are injected dependencies (i.e., not defined in Hydro)
        uint16_t nwired = 0;
        
        // dependency injection
        inject(threadContext, argList, nwired, fn);
        
        // call non-void function
        return callFunction(fn, argList, result, nargs - nwired);
    }
    
    /*
	template <typename ReturnType, typename... Arg>
	bool invokeNativeFunc(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (*fn)(HvmEnv *env, Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
	{
		// prep
        prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);

		// provide environment
		args.push_front(hvalue{_env});

		// call
		return callFunction(fn, args, result, (int32_t)args.size() - 1);
	}*/

	template <typename ReturnType, typename... Arg>
	bool invokeNativeMethod(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (*fn)(hobject, Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
	{
		// prep
        prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);

		// provide instance
		//args.push_front(thisObject);
        
        // argument count
        uint16_t nargs = (uint16_t)args.size();
        
        // number of arguments that are injected dependencies (i.e., not defined in Hydro)
        uint16_t nwired = 0;
        
        // dependency injection
        inject(threadContext, thisObject, args, nwired, fn);
        
		// call
		return callMethod(fn, args, nargs - nwired);
	}
    
    template <typename ReturnType, typename... Arg>
    bool invokeNativeMethod(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (*fn)(hobject, Arg...), Closure *closure, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject)
    {
        // prep
        std::list<hvalue> argList;
        prepVmCall(threadContext, callingContext, closure, args, argList, nullptr, thisObject);
        
        // provide instance
        //args.push_front(thisObject);
        
        // argument count
        uint16_t nargs = (uint16_t)args.size();
        
        // number of arguments that are injected dependencies (i.e., not defined in Hydro)
        uint16_t nwired = 0;
        
        // dependency injection
        inject(threadContext, thisObject, args, nwired, fn);
        
        // call
        return callMethod(fn, argList, nargs - nwired);
    }
    /*
	template <typename ReturnType, typename... Arg>
	bool invokeNativeMethod(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (*fn)(HvmEnv *, hobject, Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
	{
		// prep
		prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);

		// provide instance and environment
		args.push_front(thisObject);
		args.push_front(hvalue{_env});

		// call non-void method
		return callMethod(fn, args, result, (int32_t)args.size() - 2);
	}

	template <typename ReturnType, typename... Arg>
	bool invokeNativeMethod(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (*fn)(hvalue, Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
	{
		// prep
		prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);

		// provide instance
		args.push_front(thisObject);

		// call non-void method
		return callMethod(thisObject, fn, args, result, (int32_t)args.size() - 1);
	}

	template <typename ReturnType, typename... Arg>
	bool invokeNativeMethod(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (*fn)(HvmEnv *, hvalue, Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
	{
		// prep
		prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);

		// provide instance and environment
		args.push_front(thisObject);
		args.push_front(hvalue{_env});

		// call non-void method
		return callMethod(fn, args, result, (int32_t)args.size() - 2);
	}
    */
	template <typename ReturnType, typename ClassType, typename... Arg>
	bool invokeNativeMethod(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (ClassType::*fn)(Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
	{
		// prep
		prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);
        
        // argument count
        uint16_t nargs = (uint16_t)args.size();
        
        // number of arguments that are injected dependencies (i.e., not defined in Hydro)
        uint16_t nwired = 0;
        
        // dependency injection
        inject(threadContext, args, nwired, fn);
        
		// call
		return callMethod(thisObject, fn, args, result, nargs - nwired);
	}
    
    template <typename ReturnType, typename ClassType, typename... Arg>
    bool invokeNativeMethod(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (ClassType::*fn)(Arg...), Closure *closure, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject)
    {
        // prep
        std::list<hvalue> argList;
        prepVmCall(threadContext, callingContext, closure, args, argList, nullptr, thisObject);
        
        // argument count
        uint16_t nargs = (uint16_t)args.size();
        
        // number of arguments that are injected dependencies (i.e., not defined in Hydro)
        uint16_t nwired = 0;
        
        // dependency injection
        inject(threadContext, argList, nwired, fn);
        
        // call
        return callMethod(thisObject, fn, argList, result, nargs - nwired);
    }
    /*
	template <typename ReturnType, typename ClassType, typename... Arg>
	bool invokeNativeMethod(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (ClassType::*fn)(HvmEnv *env, Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
	{
		// prep
		prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);
        
		// provide environment
		args.push_front(hvalue{_env});

		// call non-void method
		return callMethod(thisObject, fn, args, result, (int32_t)args.size() - 1);
	}*/
    
    //bool fireEvent(EventType *eventType,)
    
    bool triggerHydroAction(HvmContext *threadContext, class HAction *callingContext, Closure *closure, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject);
    
    template <typename ReturnType, typename ClassType, typename... Arg>
    bool triggerNativeAction(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (ClassType::*fn)(Arg...), Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
    {
        // prep
        //prepTriggerCall(threadContext, callingContext, closure, args, nullptr, thisObject);
        
        // call
        return callMethod(thisObject, fn, args, result, (int32_t)args.size());
    }
    
    template <typename ReturnType, typename ClassType, typename... Arg>
    bool triggerNativeAction(HvmContext *threadContext, RuntimeContext *callingContext, ReturnType (ClassType::*fn)(HvmEnv *env, Arg...), Closure *closure, std::map<std::string, hvalue> &params, hvalue &result, hvalue thisObject)
    {
        // prep
        prepVmTrigger(threadContext, callingContext, closure, params, nullptr, thisObject);
        
        // provide environment
        //args.push_front(hvalue{_env});

        // call non-void method
        //return callMethod(thisObject, fn, args, result, (int32_t)args.size() - 1);
        return false;
    }
    
	void invokeTarget(HvmContext *threadContext, hvalue callable, std::list<hvalue> &args, hvalue &result, hvalue thisObject);
    
    void throwError(HvmContext *cxt, hvalue error);
    
    template <typename ...Arg>
    void throwError(HvmContext *cxt, hclass errorType, Arg...args);
    
    void runtimeError(HvmContext *cxt, std::string text = "", int32_t code = 0);
    
    bool getProperty(HvmContext *cxt, hobject instance, std::string name, hvalue &value);
    
    bool setProperty(HvmContext *cxt, hobject instance, std::string name,  hvalue &value);
    
    bool getIndex(HvmContext *cxt, hobject instance, hvalue key, hvalue &value);
    
    bool setIndex(HvmContext *cxt, hobject instance, hvalue key,  hvalue &value);
    
    bool callProperty(HvmContext *cxt, std::string name, hobject instance, std::list<hvalue> &args, hvalue &result);
};

} // namespace hydro

#endif /* __h3o_VM__ */
