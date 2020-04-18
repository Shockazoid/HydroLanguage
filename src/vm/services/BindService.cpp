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

#include "BindService.hpp"

namespace hydro
{

Closure *BindService::createClosure(const VM_Object *data) const
{
    Closure *closure = new Closure{};
    closure->data = data;
    return closure;
}
/*
hvalue BindService::ifNotExists(Runtime *runtime, const VM_Class *vclass) const
{
    if(hvalue result = hvalue{runtime->find(vclass->name)})
        return result;
    
    hclass cls = makeClass(vclass);
    runtime->bind(vclass->name, cls);
    return cls;
}

hvalue BindService::ifNotExists(Runtime *runtime, const VM_Func *vfunc) const
{
    if(hvalue result = hvalue{runtime->find(vfunc->name)})
        return result;
    
    hfunction fn = createFunction(vfunc);
    runtime->bind(vfunc->name, fn);
    return fn;
}

hvalue BindService::ifNotExists(Runtime *runtime, const ActionData *vact) const
{
    if(hvalue result = hvalue{runtime->find(vact->name)})
        return result;
    
    haction act = createAction(vact);
    runtime->bind(vact->name, act);
    return act;
}

hvalue BindService::ifNotExists(Runtime *runtime, const VM_Event *vevent) const
{
    if(hvalue result = hvalue{runtime->find(vevent->name)})
        return result;
    
    hevent e = createEvent(vevent);
    runtime->bind(vevent->name, e);
    return e;
}
*/
/*
hfunction BindService::createFunction(const VM_Func *vfunc)
{
    function_glue *glue;
    
    if(is_native(vfunc->modifier))
    {
        // native function
        glue = _env->native()->glue(vfunc);
        if(!glue)
            throw std::runtime_error{"Failed to bind native function '" + vfunc->name + "'."};
    }
    else
    {
        // hydro function
        glue = new hydro_function{};
    }
    
    glue->closure = createClosure(vfunc);
    return new Function{this, _coreService->FunctionClass(), vfunc, glue};
}

haction BindService::createAction(const ActionData *vact)
{
    // TODO if native load glue code
    action_glue *glue;
    glue = new hydro_action{};
    glue->closure = createClosure(vact);
    return new Action{this, _coreService->ActionClass(), vact, glue};
}

hevent BindService::createEvent(const VM_Event *vevent)
{
    return new EventContext{this, _coreService->EventContextClass(), vevent};
}

hfunction BindService::bindMethod(hmethod method, Object *instance)
{
    return new Function{this, _coreService->FunctionClass(), method->_vmethod, method->_glue};
}

hetype BindService::createEtype(const VM_Etype *vetype, hevent event)
{
    return new EventType{this, _coreService->EtypeClass(), vetype, event};
}

hclass BindService::makeClass(const VM_Class *vclass)
{
    ClassAllocator *allocator;
    
    if(is_native(vclass->modifier))
    {
        // native class
        allocator = _env->native()->allocator(vclass);
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
        if(hclass super = result.tryCast<Class>())
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
    
    return new Class{this, _coreService->ClassClass(), vclass, superClass, allocator};
}

hconstructor BindService::createConstructor(const VM_Constructor *vconstruct, hclass ownerClass)
{
    function_glue *glue;
    
    if(is_native(vconstruct->modifier))
    {
        // native constructor
        glue = tryFindGlue(vconstruct);
        
        if(!glue)
            throw std::runtime_error{"Failed to bind native constructor on type '" + vconstruct->owner->name + "'."};
    }
    else
    {
        // hydro constructor
        glue = new hydro_method{createClosure(vconstruct)};
    }
    
    glue->closure = createClosure(vconstruct);
    return new Constructor{this, _coreService->ConstructorClass(), vconstruct, ownerClass, glue};
}

hproperty BindService::createProperty(const VM_Property *vproperty, hclass ownerClass)
{
    property_glue *glue;
    
    if(is_native(vproperty->modifier))
    {
        // native property
        if(!vproperty->getter && !vproperty->setter)
        {
            glue = tryFindGlue(vproperty);
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
    
    return new Property{this, _coreService->PropertyClass(), vproperty, ownerClass, glue};
}

hgetter BindService::createGetter(const VM_Getter *vgetter, hproperty ownerProperty)
{
    function_glue *glue;
    if(is_native(vgetter->property->modifier))
    {
        // native getter
        glue = tryFindGlue(vgetter);
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
    return new Getter{this, _coreService->GetterClass(), vgetter, ownerProperty, glue};
}

hsetter BindService::createSetter(const VM_Setter *vsetter, hproperty ownerProperty)
{
    function_glue *glue;
    
    if(is_native(vsetter->property->modifier))
    {
        // native getter
        glue = tryFindGlue(vsetter);
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
    return new Setter{this, _coreService->SetterClass(), vsetter, ownerProperty, glue};
}

hmethod BindService::createMethod(const VM_Method *vmethod, hclass ownerClass)
{
    function_glue *glue;
    
    if(is_native(vmethod->modifier))
    {
        // native method
        glue = tryFindGlue(vmethod);
        
        if(!glue)
            throw std::runtime_error{"Failed to bind native method '" + vmethod->name + "' on type '" + vmethod->owner->name + "'."};
    }
    else
    {
        // hydro method
        glue = new hydro_method{createClosure(vmethod)};
    }
    
    glue->closure = createClosure(vmethod);
    return new Method{this, _coreService->MethodClass(), vmethod, ownerClass, glue};
}
*/

} // namespace hydro
