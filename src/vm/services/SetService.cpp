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

#include "SetService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

void SetService::globalFn(std::string name, function_glue *glue) const
{
    // create function data
    VM_Func *vfunc = new VM_Func{};
    vfunc->name = "";                   // anonymous
    vfunc->package = "";               // not applicable
    vfunc->startAddr = vfunc->endAddr = 0; // not applicable
    vfunc->modifier = mod_public;
    vfunc->returnType = new NilTypeSpecifier{};
    vfunc->nargs = 0;
    vfunc->nlocals = 0;
    vfunc->locals = new VMObject *[0];
    vfunc->chunk = nullptr;
    vfunc->nativeBound = true;
    vfunc->nhandlers = 0;
    vfunc->errorHandlers = new ErrorHandler*[0];
    
    // create closure
    Closure *closure = new Closure{};
    closure->data = vfunc;
    glue->closure = closure;

    // create runtime function
    hvalue func = new HFunction{_env, _env->core()->FunctionClass(), vfunc, glue};

    if (!_env->globals()->set(name, func))
    {
        LocalVar *var = new LocalVar{};
        var->name = name;
        var->type = new NilTypeSpecifier{};
        _env->globals()->define(var, func);
    }
}

void SetService::global(std::string name, hvalue value) const
{
    MemorySpace *globals = _env->globals();
    
    if (!globals->set(name, value))
    {
        LocalVar *var = new LocalVar{};
        var->name = name;
        var->type = new NilTypeSpecifier{};
        globals->define(var);
        globals->slots[globals->size() - 1]->content = value;
    }
}

void SetService::property(hobject instance, std::string name, hvalue value) const
{
    _vm->setProperty(_env->thread()->current(), instance, name, value);
}

void SetService::index(hobject instance, hvalue key, hvalue value) const
{
    _vm->setIndex(_env->thread()->current(), instance, key, value);
}

} // namespace hydro
