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

#include "CoreService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

void CoreService::setupCore()
{
    //if(_sdkPath.empty())
    //    throw std::runtime_error{"SDK path cannot be empty"};
    
    if(_ObjectClass)
        return;
    
    /**
     * Create Class Data
     */
    
    // system/Object
    ClassData *vobject = new ClassData{};
    vobject->base = nullptr;       // cannot have a base class
    vobject->name = "system/Object"; // @system::Object
    vobject->modifier = mod_public | mod_dynamic;

    // system/Class
    ClassData *vclass = new ClassData{};
    vclass->base = vobject;
    vclass->name = "system/Class"; // @system::Class
    vclass->modifier = mod_public | mod_final | mod_native;

    // system/HString
    ClassData *vstring = new ClassData{};
    vstring->base = vobject;
    vstring->name = "system/String";
    vstring->modifier = mod_public | mod_final | mod_native;

    // system/Function
    ClassData *vfunc = new ClassData{};
    vfunc->base = vobject;
    vfunc->name = "system/Function";
    vfunc->modifier = mod_public | mod_final | mod_native;
    
    // system/Action
    ClassData *vact = new ClassData{};
    vact->base = vobject;
    vact->name = "system/Action";
    vact->modifier = mod_public | mod_final | mod_native;
    
    // system/List
    ClassData *vlist = new ClassData{};
    vlist->base = vobject;
    vlist->name = "system/List";
    vlist->modifier = mod_public | mod_final | mod_native;

    // system/Dictionary
    ClassData *vdict = new ClassData{};
    vdict->base = vobject;
    vdict->name = "system/Dictionary";
    vdict->modifier = mod_public | mod_final | mod_native;

    // system/Json
    ClassData *vjson = new ClassData{};
    vjson->base = vobject;
    vjson->name = "system/Json";
    vjson->modifier = mod_public | mod_final | mod_native;

    // system/Xml
    ClassData *vxml = new ClassData{};
    vxml->base = vobject;
    vxml->name = "system/Xml";
    vxml->modifier = mod_public | mod_final | mod_native;

    // system/Regex
    ClassData *vregex = new ClassData{};
    vregex->base = vobject;
    vregex->name = "system/Regex";
    vregex->modifier = mod_public | mod_native;
    
    // system/Date
    ClassData *vdate = new ClassData{};
    vregex->base = vobject;
    vregex->name = "system/Date";
    vregex->modifier = mod_public | mod_native;
    
    // system/Constructor
    ClassData *vconstructor = new ClassData{};
    vconstructor->base = vobject;
    vconstructor->name = "system/Constructor";
    vconstructor->modifier = mod_public | mod_final | mod_native;
    
    // system/Method
    ClassData *vmethod = new ClassData{};
    vmethod->base = vobject;
    vmethod->name = "system/Method";
    vmethod->modifier = mod_public | mod_final | mod_native;
    
    // system/Property
    ClassData *vproperty = new ClassData{};
    vproperty->base = vobject;
    vproperty->name = "system/Property";
    vproperty->modifier = mod_public | mod_final | mod_native;
    
    // system/Getter
    ClassData *vgetter = new ClassData{};
    vgetter->base = vobject;
    vgetter->name = "system/Getter";
    vgetter->modifier = mod_public | mod_final | mod_native;

    // system/Setter
    ClassData *vsetter = new ClassData{};
    vsetter->base = vobject;
    vsetter->name = "system/Getter";
    vsetter->modifier = mod_public | mod_final | mod_native;
    
    // system/Thread
    ClassData *vthread = new ClassData{};
    vthread->base = vobject;
    vthread->name = "system/Thread";
    vthread->modifier = mod_public | mod_native;
    
    // system/Context
    ClassData *vcontext = new ClassData{};
    vcontext->base = vobject;
    vcontext->name = "system/Context";
    vcontext->modifier = mod_public | mod_native;
    
    // system/EventContext
    ClassData *vevent = new ClassData{};
    vevent->base = vobject;
    vevent->name = "system/EventContext";
    vevent->modifier = mod_public | mod_native | mod_final;
    
    // system/EventType
    ClassData *vetype = new ClassData{};
    vetype->base = vobject;
    vetype->name = "system/EventContext";
    vetype->modifier = mod_public | mod_native | mod_final;
    
    // system/Interface
    ClassData *vinterface = new ClassData{};
    vinterface->base = vobject;
    vinterface->name = "system/Interface";
    vinterface->modifier = mod_public | mod_native | mod_final;
    
    /**
     * Reserve Core
     */
    
    // class
    _ClassClass = reserve(vclass);
    
    // object
    _ObjectClass = reserve(vobject, nullptr); // Note: object class will be null since system::Object does not have a base type
    
    _ClassClass->_superClass = _ObjectClass;
    
    // interface
    _InterfaceClass = reserve(vinterface, _ObjectClass);
    
    // function
    _FunctionClass = reserve(vfunc, _ObjectClass);
    
    // action
    _ActionClass = reserve(vact, _ObjectClass);

    // string
    _StringClass = reserve(vstring, _ObjectClass);
    
    // list
    _ListClass = reserve(vlist, _ObjectClass);
    
    // dictionary
    _DictionaryClass = reserve(vdict, _ObjectClass);

    // json
    _JsonClass = reserve(vjson, _ObjectClass);
    
    // xml
    _XmlClass = reserve(vxml, _ObjectClass);
    
    // regex
    _RegexClass = reserve(vregex, _ObjectClass);
    
    // date
    _DateClass = reserve(vdate, _ObjectClass);
    
    // context
    _ContextClass = reserve(vcontext, _ObjectClass);
    
    // event context
    _EventContextClass = reserve(vevent, _ContextClass);
    
    /**
     * Reserve Reflection
     */
    
    // constructor
    _ConstructorClass = reserve(vconstructor, _ObjectClass);
    
    // property
    _PropertyClass = reserve(vproperty, _ObjectClass);
    
    // getter
    _GetterClass = reserve(vgetter, _ObjectClass);
    
    // setter
    _SetterClass = reserve(vsetter, _ObjectClass);
    
    // method
    _MethodClass = reserve(vmethod, _ObjectClass);
    
    // thread
    _ThreadClass = reserve(vthread, _ObjectClass);

    std::string path;
    
    /**
     * Load Core Hydro Files
     */
    
    // load Object
    loadDefinitionFromSdk("system/Object");
    
    // load Class
    loadDefinitionFromSdk("system/Class");
    
    // load Function
    loadDefinitionFromSdk("system/Function");
    
    // load Action
    loadClassFromSdk("system/Action");
    
    // load HString
    loadDefinitionFromSdk("system/String");
    
    // load List
    loadDefinitionFromSdk("system/List");
    
    // load Dictionary
    loadDefinitionFromSdk("system/Dictionary");

    // load Json
    loadDefinitionFromSdk("system/Json");
    
    // load Date
    loadDefinitionFromSdk("system/Date");

    // load RuntimeError
    _ErrorClass = loadClassFromSdk("system/Error");
    
    // load RuntimeError
    _RuntimeErrorClass = loadClassFromSdk("system/RuntimeError");
    
    // load Context
    _ContextClass = loadClassFromSdk("system/Context");
    
    // load EventContext
    _EventContextClass = loadClassFromSdk("system/EventContext");
    
    // load EventType
    _EtypeClass = loadClassFromSdk("system/EventType");
    
    // load Event
    //_EventEvent = loadEventFromSdk("system/Event");
    
    // load KeyboardEvent
    //_KeyboardEvent = loadEventFromSdk("system/ui/KeyboardEvent")

    // load MouseEvent
    //_MouseEvent = loadEventFromSdk("system/ui/MouseEvent")
    
    /**
     * Reflection
     */
    
    // load Constructor
    loadDefinitionFromSdk("system/Constructor");
    
    // load Property
    loadDefinitionFromSdk("system/Property");
    
    // load Getter
    loadDefinitionFromSdk("system/Getter");
    
    // load Setter
    loadDefinitionFromSdk("system/Setter");
    
    // load Method
    loadDefinitionFromSdk("system/Method");
    
    // load Thread
    loadDefinitionFromSdk("system/Thread");
    
    /**
     * load cmd()
     */
    loadDefinitionFromSdk("system/cmd");
    
    /**
     * load eval()
     */
    loadDefinitionFromSdk("system/eval");
    
    /**
     * load print()
     */
    loadDefinitionFromSdk("system/print");
    
    /**
     * load delay()
     */
    loadDefinitionFromSdk("system/delay");
    
    /**
     * load exit()
     */
    loadDefinitionFromSdk("system/exit");
    
    /**
     * Application
     */
    // load ApplicationDomain
    //loadClassFromSdk("system/app/ApplicationDomain");
    
}

hobject CoreService::loadDefinitionFromSdk(std::string assemblyName)
{
    /*
    std::string path = _sdkPath + '/' + assemblyName + ".hydro";
    hvalue result;
    
    // try resolve class first
    hobject target = _env->appDomain()->system()->find(assemblyName);
    
    if(target)
        return target; // success!
    
    // try load file
    _env->run()->hydroFile(path, result, nullptr, _env->appDomain()->system());
    
    // try resolve class again
    target = _env->appDomain()->system()->find(assemblyName);
    
    if(!target)
        throw std::runtime_error{"Failed to load definition '" + assemblyName + "'."};
    */
    // try resolve class first
    hobject target = _env->appDomain()->system()->find(assemblyName);
    
    if(target)
        return target; // already loaded!
    
    // load source
    Source *source = _env->nativeLib()->resolve(assemblyName);
    
    if(!source)
        return nullptr; // fail
    
    // try run source
    _env->run()->hydro(source);
    
    // try resolve class again
    target = _env->appDomain()->system()->find(assemblyName);

    if(!target)
        throw std::runtime_error{"Failed to load definition '" + assemblyName + "'."};

    // success!
    return target;
}

hclass CoreService::loadClassFromSdk(std::string assemblyName)
{
    if(hobject o = loadDefinitionFromSdk(assemblyName))
    {
        if(hclass type = o.tryCast<HClass>())
            return type;
        
        throw std::runtime_error{"Loaded definition '" + assemblyName + "' from SDK is not a class."};
    }
    
    // code never reached (error will be thrown)
    return nullptr;
}

ClassData *CoreService::loadReserved(std::string assemblyName) const
{
    for(HClass *c : _reserved)
        if(c->_vclass->name == assemblyName)
            return const_cast<ClassData *>(c->_vclass);
    
    // fail
    return nullptr;
}

hclass CoreService::resolveAndRemoveReservedClass(const ClassData *reservedVclass) const
{
    for(HClass *c : _reserved)
    {
        if(c->_vclass == reservedVclass)
        {
            ClassAllocator *allocator = nullptr;
            
            if(is_native(c->_vclass->modifier))
            {
                allocator = _env->native()->allocator(c->_vclass);
                if(!allocator && c->_vclass->construct)
                    throw std::runtime_error{"Failed to bind native class '" + c->_vclass->name + "'."};
            }
            
            hclass clss = c;
            self->_reserved.remove(c);
            c->_allocator = allocator;
            c->_vclass->chunk->runtime->bind(c->_vclass->name, clss); // bind to runtime
            c->setup(); // late initialization
            return c;
        }
    }
    
    // fail
    return nullptr;
}

hclass CoreService::reserve(const ClassData *vclass)
{
    std::string package = get_package_name(vclass->name);
    HClass *clss = new HClass{_env, vclass};
    _reserved.push_back(clss);
    return clss;
}

hclass CoreService::reserve(const ClassData *vclass, hclass superClass)
{
    std::string package = get_package_name(vclass->name);
    HClass *clss = new HClass{_env, _ClassClass, vclass}; // late initialization constructor
    clss->_superClass = superClass;
    _reserved.push_back(clss);
    return clss;
}

} // namespace hydro
