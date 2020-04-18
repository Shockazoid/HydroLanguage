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

#include "EvalService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{


bool EvalService::code(std::string hydroCode, hvalue &result, MemorySpace *scopeToUse) const
{
    HvmContext *cxt = _env->thread()->current();
    
    // init scope
    if (!scopeToUse)
        scopeToUse = new MemorySpace{_env->globals()}; // has own scope

    // create runtime
    Runtime *runtime = new Runtime{_env->appDomain()->system()};
    
    // compiler settings
    HydroCompilerSettings *settings = new HydroCompilerSettings{};
    settings->srcPath = _srcPath;
    settings->sdkPath = _sdkPath;
    settings->libPath = _libPath;
    
    // create compiler
    HydroCompiler *compiler = new HydroCompiler{settings, _env, runtime};
    delete settings;
    compiler->debug(_env->debug());
    compiler->compile(new StringSource{hydroCode, "(eval string)"}, h3o_compile_eval);

    if (compiler->failed())
    {
        // compilation error
        std::string err = compiler->errorTextAt(0);
        std::string src = compiler->errorSourceAt(0);
        uint32_t line = compiler->errorLineAt(0);
        delete compiler;
        throw new ParseError{err, src, line};
    }

    Chunk *program = compiler->result();
    delete compiler;

    try
    {
        return _vm->exec(cxt, program, result, scopeToUse);
    }
    catch (VM::hvm_uncaught_error err)
    {
        if(_env->typing()->check(_env->appDomain()->system()->findClass("system/Error"), err.target))
        {
            hvalue msg;
            if(_env->get()->property(err.target, "message", msg))
            {
                std::cout << "Error: " << _env->cast()->string(msg) << std::endl;
                return false;
            }
        }

        // default
        std::cerr << "Terminated with uncaught exception of " + _env->util()->describe(err.target)  + "." << std::endl;
        return false;
    }
}

hvalue EvalService::code(std::string hydroCode, MemorySpace *scopeToUse) const
{
    hvalue result;
    
    if(code(hydroCode, result, scopeToUse))
        return result;
    
    return undefined;
}

bool EvalService::source(Source *src, hvalue &result, MemorySpace *scopeToUse) const
{
    HvmContext *cxt = _env->thread()->current();

    // init scope
    if (!scopeToUse)
        scopeToUse = new MemorySpace{_env->globals()};

    // create runtime
    Runtime *runtime = new Runtime{_env->appDomain()->system()};

    // compiler settings
    HydroCompilerSettings *settings = new HydroCompilerSettings{};
    //settings->srcPath = _srcPath;
    //settings->sdkPath = _sdkPath;
    //settings->libPath = _libPath;

    // create compiler
    HydroCompiler *compiler = new HydroCompiler{settings, _env, runtime};
    compiler->appendLibrary(_env->nativeLib());
    delete settings;
    compiler->debug(_env->debug());
    compiler->compile(src, h3o_compile_eval);

    if (compiler->failed())
    {
        // compilation error
        std::string err = compiler->errorTextAt(0);
        std::string src = compiler->errorSourceAt(0);
        uint32_t line = compiler->errorLineAt(0);
        delete compiler;
        throw new ParseError{err, src, line};
    }

    Chunk *program = compiler->result();
    delete compiler;

    try
    {
        return _vm->exec(cxt, program, result, scopeToUse);
    }
    catch (VM::hvm_uncaught_error err)
    {
        if(_env->typing()->check(_env->appDomain()->system()->findClass("system/Error"), err.target))
        {
            hvalue text;
            if(_env->get()->property(err.target, "message", text))
            {
                std::cout << "Error: " << _env->cast()->string(text) << std::endl;
                return false;
            }
        }
        
        // default
        std::cerr << "Terminated with uncaught exception of " + _env->util()->describe(err.target)  + "." << std::endl;
        return false;
    }
}

hvalue EvalService::source(Source *src, MemorySpace *scopeToUse) const
{
    hvalue result;
    
    if(source(src, result, scopeToUse))
        return result;
    
    return undefined;
}


} // namespace hydro
