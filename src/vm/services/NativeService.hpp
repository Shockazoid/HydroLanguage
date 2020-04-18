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

#ifndef __h3o_NativeService__
#define __h3o_NativeService__

#include <vector>

#include "RuntimeService.hpp"
#include "NativeResource.hpp"
#include "FunctionResource.hpp"
#include "ClassResource.hpp"

namespace hydro
{

struct NativeService final : RuntimeService
{
    friend class HvmEnv;
    friend struct CoreService;
    
private:
    std::vector<NativeResource *> _resources;

    template <typename Type = void>
    void checkResourceConflict(std::string qualifiedName)
    {
        return;
        for (NativeResource *res : _resources)
        {
            if (typeid(Type) != typeid(void) && res->type() == typeid(Type))
                throw std::runtime_error("The supplied class is already registered attempted as '" + qualifiedName + "'.");
            else if (res->qualifiedName() == qualifiedName)
                throw std::runtime_error("The supplied qualified class name, '" + qualifiedName + "' is already registered.");
        }
    }

    ClassAllocator *allocator(const VM_Class *vclass) const
    {
        if(is_native(vclass->modifier))
        {
            std::string qname = vclass->name;

            for(NativeResource *res : _resources)
            {
                if(NativeClassResource *c = dynamic_cast<NativeClassResource *>(res))
                    if(c->assemblyName() == qname)
                        return c->allocator();
            }
        }
        
        // fail
        return nullptr;
    }

    function_glue *glue(const FuncData *vfunc) const
    {
        if(is_native(vfunc->modifier))
        {
            std::string qname = vfunc->name;

            for(NativeResource *res : _resources)
            {
                if(NativeFunctionResource *fn = dynamic_cast<NativeFunctionResource *>(res))
                    if(fn->assemblyName() == qname)
                        return fn->glue();
            }
        }
        
        // fail
        return nullptr;
    }

    function_glue *glue(const ConstructorData *vconstruct) const
    {
        if(is_native(vconstruct->modifier))
        {
            std::string qname = vconstruct->owner->name;

            for(NativeResource *res : _resources)
            {
                if(NativeClassResource *c = dynamic_cast<NativeClassResource *>(res))
                    if(c->assemblyName() == qname)
                        return c->constructor() ? c->constructor()->glue() : nullptr;
            }
        }
        
        // fail
        return nullptr;
    }

    property_glue *glue(const PropertyData *vproperty) const
    {
        if(is_native(vproperty->modifier))
        {
            std::string qname = vproperty->owner->name;

            for(NativeResource *res : _resources)
            {
                if(NativeClassResource *c = dynamic_cast<NativeClassResource *>(res))
                    if(c->assemblyName() == qname)
                        if(NativePropertyResource *prop = c->lookupProperty(vproperty->name))
                            return prop->glue();
            }
        }
        
        // fail
        return nullptr;
    }

    function_glue *glue(const GetterData *vgetter) const
    {
        const PropertyData *vproperty = vgetter->property;
        
        if(is_native(vproperty->modifier))
        {
            std::string qname = vproperty->owner->name;

            for(NativeResource *res : _resources)
            {
                if(NativeClassResource *c = dynamic_cast<NativeClassResource *>(res))
                    if(c->assemblyName() == qname)
                        if(NativePropertyResource *prop = c->lookupProperty(vproperty->name))
                            if(NativeFunctionResource *get = prop->getter())
                                return get->glue();
            }
        }
        
        // fail
        return nullptr;
    }

    function_glue *glue(const SetterData *vsetter) const
    {
        const PropertyData *vproperty = vsetter->property;
        
        if(is_native(vproperty->modifier))
        {
            std::string qname = vproperty->owner->name;

            for(NativeResource *res : _resources)
            {
                if(NativeClassResource *c = dynamic_cast<NativeClassResource *>(res))
                    if(c->assemblyName() == qname)
                        if(NativePropertyResource *prop = c->lookupProperty(vproperty->name))
                            if(NativeFunctionResource *set = prop->setter())
                                return set->glue();
            }
        }
        
        // fail
        return nullptr;
    }

    function_glue *glue(const MethodData *vmethod) const
    {
        if(is_native(vmethod->modifier))
        {
            std::string qname = vmethod->owner->name;

            for(NativeResource *res : _resources)
            {
                if(NativeClassResource *c = dynamic_cast<NativeClassResource *>(res))
                    if(c->assemblyName() == qname)
                        if(NativeFunctionResource *meth = c->lookupMethod(vmethod->name))
                            return meth->glue();
            }
        }
        
        // fail
        return nullptr;
    }

public:
    NativeService(HvmEnv *env) : RuntimeService{env}, _resources{} {}
    virtual ~NativeService() {}
    
    template <typename ReturnType, typename... Arg>
    void function(std::string qualifiedName, ReturnType (*fn)(Arg...))
    {
        checkResourceConflict(qualifiedName);
        _resources.push_back(new FunctionResource{qualifiedName, fn});
    }
    
    template <typename ClassType>
    ClassResource<ClassType> *type(std::string qualifiedName)
    {
        checkResourceConflict<ClassType>(qualifiedName);
        ClassResource<ClassType> *res = new ClassResource<ClassType>{qualifiedName};
        assert(res);
        _resources.push_back(res);
        return res;
    }

    template <typename ClassType, typename... Arg>
    ClassResource<ClassType> *type(std::string qualifiedName, void (*constructor)(object_ptr<HObject>, Arg...))
    {
        checkResourceConflict<ClassType>(qualifiedName);
        ClassResource<ClassType> *res = new ClassResource<ClassType>{qualifiedName, constructor};
        assert(res);
        _resources.push_back(res);
        return res;
    }

    template <typename ClassType, typename... Arg>
    ClassResource<ClassType> *type(std::string qualifiedName, void (*constructor)(object_ptr<ClassType>, Arg...))
    {
        checkResourceConflict<ClassType>(qualifiedName);
        ClassResource<ClassType> *res = new ClassResource<ClassType>{qualifiedName, constructor};
        assert(res);
        _resources.push_back(res);
        return res;
    }

    template <typename ClassType, typename... Arg>
    ClassResource<ClassType> *type(std::string qualifiedName, void (*constructor)(hvalue, Arg...))
    {
        checkResourceConflict<ClassType>(qualifiedName);
        ClassResource<ClassType> *res = new ClassResource<ClassType>{qualifiedName, constructor};
        assert(res);
        _resources.push_back(res);
        return res;
    }
    
    template <typename ClassType, typename... Arg>
    ClassResource<ClassType> *type(std::string qualifiedName, void (ClassType::*constructor)(Arg...))
    {
        checkResourceConflict<ClassType>(qualifiedName);
        ClassResource<ClassType> *res = new ClassResource<ClassType>{qualifiedName, constructor};
        assert(res);
        _resources.push_back(res);
        return res;
    }
    
};

} // namespace hydro

#endif /* __h3o_NativeService__ */
