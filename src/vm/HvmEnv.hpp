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

#ifndef __h3o_HvmEnv__
#define __h3o_HvmEnv__

#define HVM_VERSION "0.1.0"

#include <string>
#include <thread>
#include <list>

#include "HvmInitOptions.hpp"
#include "defaults.hpp"
#include "VM.hpp"
#include "HvmContext.hpp"
#include "Runtime.hpp"
#include "glue.hpp"
#include "HvmExtension.hpp"
#include "services/BindService.hpp"
#include "services/CoreService.hpp"
#include "services/NativeService.hpp"
#include "services/LogService.hpp"
#include "services/MakeService.hpp"
#include "services/GetService.hpp"
#include "services/SetService.hpp"
#include "services/CallService.hpp"
#include "services/TriggerService.hpp"
#include "services/CastService.hpp"
#include "services/ErrorService.hpp"
#include "services/ThrowService.hpp"
#include "services/EvalService.hpp"
#include "services/CompileService.hpp"
#include "services/GCService.hpp"
#include "services/RunService.hpp"
#include "services/UtilityService.hpp"
#include "services/TypingService.hpp"
#include "services/ApplicationDomainService.hpp"
#include "services/FireService.hpp"
#include "services/EventService.hpp"
#include "services/ActionService.hpp"
#include "services/OnService.hpp"
#include "../system/HFunction.hpp"
#include "../system/HAction.hpp"
#include "../system/HClass.hpp"
#include "../system/HConstructor.hpp"
#include "../system/HProperty.hpp"
#include "../system/HMethod.hpp"
#include "../system/HDictionary.hpp"
#include "../system/HString.hpp"
#include "../system/HList.hpp"
#include "../system/HJson.hpp"
#include "../system/HRegex.hpp"
#include "../system/HDate.hpp"
#include "../system/HError.hpp"
#include "../system/HEventContext.hpp"
#include "../system/HEventType.hpp"
#include "../system/network/Url.hpp"
#include "../system/network/HttpRequest.hpp"
#include "../system/network/HttpLoader.hpp"
#include "../system/network/HttpRequest.hpp"

namespace hydro
{

extern const int hydro_script_size;
extern const char **hydro_script_data;
extern const char **hydro_script_path;
extern const char **hydro_script_qname;

class hvalue;

class HvmEnv final
{
	friend class H3oAssembler;
	friend class VM;
	friend class HObjectContext;
    friend class HFunction;
    friend class HContext;
    friend class HEventContext;
    friend class HAction;
    friend class HClass;
    friend class HProperty;
    friend struct RuntimeService;
    friend struct ThreadService;
    friend HvmEnv *hvm_setup(HvmInitOptions *options);
    friend HvmEnv *hvm_get_instance();

public:
	~HvmEnv();

	HvmContext *createNewHeap();

    const HvmExtension<CoreService> &core() const { return _coreService; }

	const HvmExtension<LogService> &log() const { return _logService; }
    
    void log(hvalue value) const { return _logService->print(value); }
    
    HvmExtension<NativeService> &native() { return _nativeService; }
    
    const HvmExtension<MakeService> &make() const { return _makeService; }
    
    template <typename ...Args>
    hobject make(hclass type, Args...args) const
    {
        return _makeService->object(type, args...);
    }
    
    hobject make(hclass type, std::list<hvalue> &args) const
    {
        return _makeService->object(type, args);
    }
    
    const HvmExtension<MakeService> &create() const { return _makeService; }
    
    template <typename ...Args>
    hobject create(hclass type, Args...args) const
    {
        return _makeService->object(type, args...);
    }
    
    hobject create(hclass type, std::list<hvalue> &args) const
    {
        return _makeService->object(type, args);
    }

    const HvmExtension<GetService> &get() const { return _getService; }
    
    const HvmExtension<SetService> &set() const { return _setService; }
    
    const HvmExtension<CallService> &call() const { return _callService; }

    template <typename ...Arg>
    hvalue call(hvalue callable, Arg...args)
    {
        return _callService->target(callable, args...);
    }

    hvalue call(hvalue callable, std::list<hvalue> &args, hvalue thisObject = nullptr)
    {
        return _callService->target(callable, args, thisObject);
    }
    
   	const HvmExtension<TriggerService> &trigger() const { return _triggerService; }
    
    const HvmExtension<FireService> &fire() const { return _fireService; }
    
    void fire(hobject target, hetype type, std::map<std::string, hvalue> &params) const;
    
    const HvmExtension<EventService> &event() const { return _eventService; }
    
    const HvmExtension<ActionService> &action() const { return _actionService; }
    
    const HvmExtension<OnService> &on() const { return _onService; }

    const HvmExtension<CastService> &cast() const { return _castService; }
    
    hobject cast(hclass type, hvalue subject) const { return _castService->type(type, subject); }

    const HvmExtension<ThreadService> &thread() const { return _threadService; }
    
	const HvmExtension<ErrorService> &error() const { return _errorService; }
    
    const HvmExtension<ThrowService> &vthrow() const { return _throwService; }

	const HvmExtension<GCService> &gc() const { return _gcService; }

	const HvmExtension<EvalService> &eval() const { return _evalService; }

	bool eval(std::string hydroCode, hvalue &result, MemorySpace *scopeToUse = nullptr) const;

    hvalue eval(std::string hydroCode, MemorySpace *scopeToUse = nullptr) const;

	bool eval(Source *source, hvalue &result, MemorySpace *scopeToUse = nullptr) const;
	
    hvalue eval(Source *source, MemorySpace *scopeToUse = nullptr) const;

	const HvmExtension<CompileService> &compile() const { return _compileService; }

	const HvmExtension<RunService> &run() const { return _runService; }

	const HvmExtension<UtilityService> &util() const { return _utilService; }
    
    const HvmExtension<TypingService> &typing() const { return _typingService; }
    
    const HvmExtension<ApplicationDomainService> &appDomain() const { return _appDomainService; }
    
	void kill();
    
	MemorySpace *globals() const { return _globals; }
    
	bool debug() const { return _debug; }
	void debug(bool value) { _debug = value; }
    
    Library *nativeLib() const { return _nativeLib; }
    
    
    private:
        HvmEnv();
        static HvmEnv *_instance; // singleton instance

        std::string _srcPath, _sdkPath, _libPath;
        MemorySpace *_globals;
        VM *_vm;
        HvmContext *_mainContext; // main
        HvmContext *_currentContext;
        std::list<HClass *> _reserved;
        bool _debug;

        // services
        HvmExtension<BindService> _bindService;
        HvmExtension<CoreService> _coreService;
        HvmExtension<LogService> _logService;
        HvmExtension<NativeService> _nativeService;
        HvmExtension<MakeService> _makeService;
        HvmExtension<GetService> _getService;
        HvmExtension<SetService> _setService;
        HvmExtension<CallService> _callService;
        HvmExtension<TriggerService> _triggerService;
        HvmExtension<FireService> _fireService;
        HvmExtension<EventService> _eventService;
        HvmExtension<ActionService> _actionService;
        HvmExtension<OnService> _onService;
        HvmExtension<CastService> _castService;
        HvmExtension<ThreadService> _threadService;
        HvmExtension<ErrorService> _errorService;
        HvmExtension<ThrowService> _throwService;
        HvmExtension<GCService> _gcService;
        HvmExtension<EvalService> _evalService;
        HvmExtension<CompileService> _compileService;
        HvmExtension<RunService> _runService;
        HvmExtension<UtilityService> _utilService;
        HvmExtension<TypingService> _typingService;
        HvmExtension<ApplicationDomainService> _appDomainService;
        Library *_nativeLib;

        void setup(HvmInitOptions *options);
        
        void initCore();
        
        const VM_Class *findClass(std::string);
        
        Closure *createClosure(const VM_Class *vclass);
        
        Closure *createClosure(const VM_Func *vfunc);
        
        hvalue createAndBindIfNotExists(Runtime *runtime, const VM_Class *vclass);
        
        hvalue createAndBindIfNotExists(Runtime *runtime, const VM_Func *vfunc);
        
        hvalue createAndBindIfNotExists(Runtime *runtime, const VM_Action *vact);
        
        hvalue createAndBindIfNotExists(Runtime *runtime, const VM_Event *vevent);
        
        hfunction createFunction(const VM_Func *vfunc);
        
        haction createAction(const VM_Action *vact);
        
        hevent createEvent(const VM_Event *vevent);
        
        hetype createEtype(const VM_Etype *vetype, hevent event);
        
        hfunction bindMethod(hmethod method, HObject *instance);
            
        hclass makeClass(const VM_Class *vclass);
        
        hconstructor createConstructor(const VM_Constructor *vconstruct, hclass ownerClass);

        hproperty createProperty(const VM_Property *vproperty, hclass ownerClass);
        hgetter createGetter(const GetterData *vgetter, hproperty ownerProperty);
        
        hsetter createSetter(const SetterData *vsetter, hproperty ownerProperty);
        
        hmethod createMethod(const VM_Method *vmethod, hclass ownerClass);
            
        template <typename X>
        hvalue toHvmObject(X x) { return hvalue{x}; }

        static HvmEnv *initialize(HvmInitOptions *options = nullptr);
        
        const HvmExtension<BindService> &bind() const { return _bindService; }
        
};

HvmEnv *hvm_setup(HvmInitOptions *options);

HvmEnv *hvm_get_instance();

} // namespace hydro

#endif /* __h3o_HvmEnv__ */
