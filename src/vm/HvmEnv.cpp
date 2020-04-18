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

#include "HvmEnv.hpp"
#include "HvmDelegate.hpp"
#include "ClassData.hpp"
#include "FuncData.hpp"
#include "../utility/classhelper.hpp"
#include "../system/HConstructor.hpp"
#include "../system/HProperty.hpp"
#include "../system/HGetter.hpp"
#include "../system/HSetter.hpp"
#include "../system/HMethod.hpp"
#include "../system/HThread.hpp"
#include "../system/hvm_eval.hpp"
#include "../system/hvm_delay.hpp"
#include "../system/hvm_exit.hpp"
#include "../system/hvm_cmd.hpp"
#include "../system/hvm_print.hpp"
#include "../system/io/HFileStream.hpp"
#include "../system/io/hvm_read.hpp"
#include "../system/io/hvm_readln.hpp"
#include "../system/io/hvm_write.hpp"
#include "../system/io/hvm_writeln.hpp"
#include "../library/StringLibrary.hpp"

namespace hydro
{

HvmEnv *HvmEnv::_instance = nullptr;

HvmEnv::HvmEnv(HvmDelegate *delegate) : _globals{nullptr}, _vm{nullptr}, _currentContext{nullptr}, _reserved{}, _delegate{delegate}, _debug{false}, _srcPath{""}, _sdkPath{""}, _libPath{""}
{
    _instance = this;
}

HvmEnv::~HvmEnv() {}

void HvmEnv::fire(hobject target, hetype type, std::map<std::string, hvalue> &params) const
{
    _fireService->event(target, type, params);
}

bool HvmEnv::eval(std::string hydroCode, hvalue &result, MemorySpace *scopeToUse) const
{
    return _evalService->code(hydroCode, result, scopeToUse);
}

hvalue HvmEnv::eval(std::string hydroCode, MemorySpace *scopeToUse) const
{
    return _evalService->code(hydroCode, scopeToUse);
}

bool HvmEnv::eval(Source *source, hvalue &result, MemorySpace *scopeToUse) const
{
    return _evalService->source(source, result, scopeToUse);
}

hvalue HvmEnv::eval(Source *source, MemorySpace *scopeToUse) const
{
    return _evalService->source(source, scopeToUse);
}

void HvmEnv::kill()
{
	// TODO gc() for all active contexts
    if(_delegate)
        _delegate->shutDown(this);
}

HvmContext *HvmEnv::createNewHeap()
{
	return _vm->newHeap();
}

void HvmEnv::setup(HvmInitOptions *options)
{
    _srcPath = options->srcPath;
    _sdkPath = options->sdkPath;
    _libPath = options->libPath;
    _globals = new MemorySpace{};
    _vm = new VM{this, _globals};
    
    auto natLib = new StringLibrary{};
    
    for(int i = 0; i < hydro_script_size; i++)
    {
        std::string script = hydro_script_data[i];
        std::string path = hydro_script_path[i];
        std::string qname = hydro_script_qname[i];
        natLib->appendScriptFromString(script, path, qname);
    }
    
    _nativeLib = natLib;
    
    // setup runtime services
    _bindService = HvmExtension<BindService>{this};
    _coreService = HvmExtension<CoreService>{this};
    _mainContext = _currentContext = createNewHeap();
    _logService = HvmExtension<LogService>{this};
    _nativeService = HvmExtension<NativeService>{this};
    _makeService = HvmExtension<MakeService>{this};
    _getService = HvmExtension<GetService>{this};
    _setService = HvmExtension<SetService>{this};
    _callService = HvmExtension<CallService>{this};
    _triggerService = HvmExtension<TriggerService>{this};
    _fireService = HvmExtension<FireService>{this};
    _eventService = HvmExtension<EventService>{this};
    _actionService = HvmExtension<ActionService>{this};
    _onService = HvmExtension<OnService>{this};
    _castService = HvmExtension<CastService>{this};
    _threadService = HvmExtension<ThreadService>{this};
    _errorService = HvmExtension<ErrorService>{this};
    _throwService = HvmExtension<ThrowService>{this};
    _gcService = HvmExtension<GCService>{this};
    _evalService = HvmExtension<EvalService>{this};
    _compileService = HvmExtension<CompileService>{this};
    _runService = HvmExtension<RunService>{this};
    _utilService = HvmExtension<UtilityService>{this};
    _typingService = HvmExtension<TypingService>{this};
    _appDomainService = HvmExtension<ApplicationDomainService>{this};
    
    /**
     * Register Native Core
     */
    // object
    _nativeService->type("system/Object", &HObject::construct);

    // class
    _nativeService->type<HClass>("system/Class")
        ->property("name", &HClass::name)
        ->property("qualifiedName", &HClass::qualifiedName)
        ->property("packageName", &HClass::packageName)
        ;

    // function
    _nativeService->type<HFunction>("system/Function")
        ->property("arity", &HFunction::arity)
        ->method("call", &HFunction::h3o_call)
    ;
    
    // action
    _nativeService->type<HAction>("system/Action")
        //->property("context", &Action::context)
        ->method("trigger", &HAction::h3o_trigger)
    ;
    
    // string
    _nativeService->type("system/String", &HString::construct)
        ->property("length", &HString::length)
        ->property("empty", &HString::empty)
        ->method("substr", &HString::substr)
    ;

    // list
    _nativeService->type("system/List", &HList::construct)
            // push()
            ->method("push", &HList::push)
            
            // pop()
            ->method("pop", &HList::pop)
            
            // get/set length
            ->property("length", &HList::getLength, &HList::setLength)
    ;

    // dictionary
    _nativeService->type("system/Dictionary", &HDictionary::construct)
        ->property("size", &HDictionary::size)
    ;

    // json
    _nativeService->type("system/Json", &HJson::construct);
    
    // xml
    //native()->type("system/Xml", &Xml::construct);

    // regular expression
    _nativeService->type("system/Regex", &HRegex::construct)
        ->property("pattern", &HRegex::pattern)
        ->method("test", &HRegex::test)
    ;
    
    // date
    _nativeService->type("system/Date", &HDate::construct);
    
    // error
    _nativeService->type("system/Error", &HError::construct)
        ->property("message", &HError::message)
        ->property("code", &HError::code)
        ->method("getStackTrace", &HError::getStackTrace)
    ;
    
    // thread
    _nativeService->type("system/Thread", &HThread::construct);
    
    // context
    _nativeService->type<HContext>("system/Context");
    
    // event context
    _nativeService->type<HEventContext>("system/EventContext");
    
    // event type
    _nativeService->type<HEventType>("system/EventType");
    
    
    /**
     * Register Reflection
     */
    
    // constructor
    _nativeService->type<HConstructor>("system/Constructor");
    
    // property
    _nativeService->type<HProperty>("system/Property");
    
    // getter
    _nativeService->type<HGetter>("system/Getter");
    
    // setter
    _nativeService->type<HSetter>("system/Setter");
    
    // method
    _nativeService->type<HMethod>("system/Method");
    
    // eval()
    _nativeService->function("system/eval", &hvm_eval);
    
    // delay()
    _nativeService->function("system/delay", &hvm_delay);
    
    // print()
    _nativeService->function("system/print", &hvm_print);
    
    // exit()
    _nativeService->function("system/exit", &hvm_exit);
    
    // cmd()
    _nativeService->function("system/cmd", &hvm_cmd);
    
    /**
     * Filesystem
     */
    
    // FileStream
    _nativeService->type("system/io/FileStream", &HFileStream::construct)
        ->method("open", &HFileStream::open)
        ->method("close", &HFileStream::close)
        ->method("read", &HFileStream::read)
        ->method("get", &HFileStream::get)
        ->property("isOpen", &HFileStream::isOpen)
        ->property("isEof", &HFileStream::isEof)
    ;
    
    // read()
    _nativeService->function("system/io/read", hvm_read);
    
    // readln()
    _nativeService->function("system/io/readln", hvm_readln);
    
    // write()
    _nativeService->function("system/io/write", hvm_write);
    
    // writeln()
    _nativeService->function("system/io/writeln", hvm_writeln);
    
    // initialize core
    _coreService->setupCore();
}

hvalue HvmEnv::createAndBindIfNotExists(Runtime *runtime, const ClassData *vclass)
{
    if(hvalue result = hvalue{runtime->find(vclass->name)})
        return result;
    
    hclass cls = makeClass(vclass);
    runtime->bind(vclass->name, cls);
    return cls;
}

hvalue HvmEnv::createAndBindIfNotExists(Runtime *runtime, const FuncData *vfunc)
{
    if(hvalue result = hvalue{runtime->find(vfunc->name)})
        return result;
    
    hfunction fn = createFunction(vfunc);
    runtime->bind(vfunc->name, fn);
    return fn;
}

hvalue HvmEnv::createAndBindIfNotExists(Runtime *runtime, const VM_Action *vact)
{
    if(hvalue result = hvalue{runtime->find(vact->name)})
        return result;
    
    haction act = createAction(vact);
    runtime->bind(vact->name, act);
    return act;
}

hvalue HvmEnv::createAndBindIfNotExists(Runtime *runtime, const EventData *vevent)
{
    if(hvalue result = hvalue{runtime->find(vevent->name)})
        return result;
    
    hevent e = createEvent(vevent);
    runtime->bind(vevent->name, e);
    return e;
}

Closure *HvmEnv::createClosure(const ClassData *clss)
{
    Closure *closure = new Closure{};
    closure->data = clss;
    return closure;
}

Closure *HvmEnv::createClosure(const FuncData *vfunc)
{
    Closure *closure = new Closure{};
    closure->data = vfunc;
    return closure;
}

hfunction HvmEnv::createFunction(const FuncData *vfunc)
{
    function_glue *glue;
    
    if(is_native(vfunc->modifier))
    {
        // native function
        glue = _nativeService->glue(vfunc);
        if(!glue)
            throw std::runtime_error{"Failed to bind native function '" + vfunc->name + "'."};
    }
    else
    {
        // hydro function
        glue = new hydro_function{};
    }
    
    glue->closure = createClosure(vfunc);
    return new HFunction{this, _coreService->FunctionClass(), vfunc, glue};
}

haction HvmEnv::createAction(const VM_Action *vact)
{
    // TODO if native load glue code
    action_glue *glue;
    glue = new hydro_action{};
    glue->closure = createClosure(vact);
    return new HAction{this, _coreService->ActionClass(), vact, glue};
}

hevent HvmEnv::createEvent(const EventData *vevent)
{
    return new HEventContext{this, _coreService->EventContextClass(), vevent};
}

hfunction HvmEnv::bindMethod(hmethod method, HObject *instance)
{
    return new HFunction{this, _coreService->FunctionClass(), method->_vmethod, method->_glue};
}

hetype HvmEnv::createEtype(const EtypeData *vetype, hevent event)
{
    return new HEventType{this, _coreService->EtypeClass(), vetype, event};
}

hclass HvmEnv::makeClass(const ClassData *vclass)
{
    ClassAllocator *allocator;
    
    if(is_native(vclass->modifier))
    {
        // native class
        allocator = _nativeService->allocator(vclass);
        if(!allocator && vclass->construct)
            throw std::runtime_error{"Failed to bind native class '" + vclass->name + "'."};
    }
    else
    {
        // hydro class
        allocator = nullptr; // will cause vm to allocate Object instance directly
    }
    
    if(vclass->base
        == vclass)
        throw std::runtime_error{"Class cannot be a subclass of itself."};
    
    hclass superClass;
    
    if(vclass->base)
    {
        hvalue result = createAndBindIfNotExists(vclass->base->chunk->runtime, vclass->base);
        if(hclass super = result.tryCast<HClass>())
            superClass = super;
        else if(is_null(result) || is_undefined(result))
            throw std::runtime_error{"Base type '" + get_qualified_name(vclass->base->name) + "' does not exist."};
        else
            throw std::runtime_error{"Base type '" + get_qualified_name(vclass->base->name) + "'is not a class."};
    }
    else if(vclass->name != "system/Object")
    {
        throw std::runtime_error{"Base class is required for type '" + get_qualified_name(vclass->name) + "'"};
    }
    
    return new HClass{this, _coreService->ClassClass(), vclass, superClass, allocator};
}

hconstructor HvmEnv::createConstructor(const ConstructorData *vconstruct, hclass ownerClass)
{
    function_glue *glue;
    
    if(is_native(vconstruct->modifier))
    {
        // native constructor
        glue = _nativeService->glue(vconstruct);
        
        if(!glue)
            throw std::runtime_error{"Failed to bind native constructor on type '" + vconstruct->owner->name + "'."};
    }
    else
    {
        // hydro constructor
        glue = new hydro_method{createClosure(vconstruct)};
    }
    
    glue->closure = createClosure(vconstruct);
    return new HConstructor{this, _coreService->ConstructorClass(), vconstruct, ownerClass, glue};
}

hproperty HvmEnv::createProperty(const PropertyData *vproperty, hclass ownerClass)
{
    property_glue *glue;
    
    if(is_native(vproperty->modifier))
    {
        // native property
        if(!vproperty->getter && !vproperty->setter)
        {
            glue = _nativeService->glue(vproperty);
            if(!glue)
                throw std::runtime_error{"Failed to bind native property '" + vproperty->name + "' on type '" + vproperty->owner->name + "'."};
        }
        else
        {
            // property will only use native getter and/or setter glue code
            glue = nullptr;
        }
    }
    else
    {
        // hydro property
        glue = new hydro_property{vproperty};
    }
    
    return new HProperty{this, _coreService->PropertyClass(), vproperty, ownerClass, glue};
}

hgetter HvmEnv::createGetter(const GetterData *vgetter, hproperty ownerProperty)
{
    function_glue *glue;
    if(is_native(vgetter->property->modifier))
    {
        // native getter
        glue = _nativeService->glue(vgetter);
        if(!glue)
            throw std::runtime_error{"Failed to bind native getter for property '" + vgetter->property->name + "' on type '" + vgetter->property->owner->name + "'."};
    }
    else
    {
        // hydro getter
        // if not auto property create glue
        glue = !vgetter->automatic ? new hydro_method{} : nullptr;
    }
    
    if(glue)
        glue->closure = createClosure(vgetter);
    return new HGetter{this, _coreService->GetterClass(), vgetter, ownerProperty, glue};
}

hsetter HvmEnv::createSetter(const SetterData *vsetter, hproperty ownerProperty)
{
    function_glue *glue;
    
    if(is_native(vsetter->property->modifier))
    {
        // native getter
        glue = _nativeService->glue(vsetter);
        if(!glue)
            throw std::runtime_error{"Failed to bind native getter for property '" + vsetter->property->name + "' on type '" + vsetter->property->owner->name + "'."};
    }
    else
    {
        // hydro setter
        // if not auto property create glue
        glue = !vsetter->automatic ? new hydro_method{} : nullptr;
    }
    
    if(glue)
        glue->closure = createClosure(vsetter);
    return new HSetter{this, _coreService->SetterClass(), vsetter, ownerProperty, glue};
}

hmethod HvmEnv::createMethod(const MethodData *vmethod, hclass ownerClass)
{
    function_glue *glue;
    
    if(is_native(vmethod->modifier))
    {
        // native method
        glue = _nativeService->glue(vmethod);
        
        if(!glue)
            throw std::runtime_error{"Failed to bind native method '" + vmethod->name + "' on type '" + vmethod->owner->name + "'."};
    }
    else
    {
        // hydro method
        glue = new hydro_method{createClosure(vmethod)};
    }
    
    glue->closure = createClosure(vmethod);
    return new HMethod{this, _coreService->MethodClass(), vmethod, ownerClass, glue};
}

HvmEnv *HvmEnv::initialize(HvmInitOptions *options)
{
	HvmEnv *env = new HvmEnv{};
	env->setup(options);
	return env;
}

HvmEnv *hvm_setup(HvmInitOptions *options)
{
    if(!HvmEnv::_instance)
    {
        HvmEnv *instance = new HvmEnv{};
        instance->setup(options);
        return instance;
    }
    return HvmEnv::_instance;
}

HvmEnv *hvm_get_instance() { return HvmEnv::_instance; }

} // namespace hydro
