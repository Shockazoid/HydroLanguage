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

#ifndef __h3o_CoreService__
#define __h3o_CoreService__

#include <list>

#include "RuntimeService.hpp"
#include "../../system/HClass.hpp"
#include "../../system/HEventContext.hpp"

namespace hydro
{

struct CoreService : RuntimeService
{
    friend class HvmEnv;
    friend class H3oAssembler;
    
private:
    CoreService *self;
    std::list<hydro::HClass *> _reserved;
    
    // core classes
    hclass _ObjectClass;
    hclass _ClassClass;
    hclass _InterfaceClass;
    hclass _FunctionClass;
    hclass _ActionClass;
    hclass _StringClass;
    hclass _ListClass;
    hclass _DictionaryClass;
    hclass _JsonClass;
    hclass _XmlClass;
    hclass _RegexClass;
    hclass _DateClass;
    hclass _ThreadClass;
    hclass _ErrorClass;
    hclass _RuntimeErrorClass;
    hclass _ContextClass;
    hclass _EventContextClass;
    hclass _EtypeClass;
    hclass _ConstructorClass;
    hclass _PropertyClass;
    hclass _GetterClass;
    hclass _SetterClass;
    hclass _MethodClass;
    
    // core events
    hevent _EventEvent;
    hevent _ProgressEvent;
    hevent _MouseEvent;
    hevent _KeyboardEvent;
    hevent _TouchEvent;
    
    void setupCore();
    hobject loadDefinitionFromSdk(std::string assemblyName);
    hclass loadClassFromSdk(std::string assemblyName);
    ClassData *loadReserved(std::string assemblyName) const;
    hclass resolveAndRemoveReservedClass(const ClassData *reservedVclass) const;
    hclass reserve(const ClassData *vclass);
    hclass reserve(const ClassData *vclass, hclass superClass);

public:
    CoreService(HvmEnv *env) : RuntimeService{env}, self{this} {}
    virtual ~CoreService() {}
    
    // core types
    hclass ObjectClass() const { return _ObjectClass; }
    hclass ClassClass() const { return _ClassClass; }
    hclass FunctionClass() const { return _FunctionClass; }
    hclass ActionClass() const { return _ActionClass; }
    hclass StringClass() const { return _StringClass; }
    hclass ListClass() const { return _ListClass; }
    hclass DictionaryClass() const { return _DictionaryClass; }
    hclass JsonClass() const { return _JsonClass; }
    hclass XmlClass() const { return _XmlClass; }
    hclass RegexClass() const { return _RegexClass; }
    hclass DateClass() const { return _DateClass; }
    hclass ErrorClass() const { return _ErrorClass; }
    hclass RuntimeErrorClass() const { return _RuntimeErrorClass; }
    hclass ContextClass() const { return _ContextClass; }
    hclass EventContextClass() const { return _EventContextClass; }
    hclass EtypeClass() const { return _EtypeClass; }
    hclass ThreadClass() const { return _ThreadClass; }
    hclass ConstructorClass() const { return _ConstructorClass; }
    hclass PropertyClass() const { return _PropertyClass; }
    hclass GetterClass() const { return _GetterClass; }
    hclass SetterClass() const { return _SetterClass; }
    hclass MethodClass() const { return _MethodClass; }
    
    // core events
    hevent EventEvent() const { return _EventEvent; }
    hevent ProgressEvent() const { return _ProgressEvent; }
    hevent MouseEvent() const { return _MouseEvent; }
    hevent KeyboardEvent() const { return _MouseEvent; }
    hevent TouchEvent() const { return _TouchEvent; }
};

} // namespace hydro

#endif /* __h3o_CoreService__ */
