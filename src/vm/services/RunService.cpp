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

#include "RunService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

hvalue RunService::hydro(Source *source, MemorySpace *scopeToUse, Runtime *runtimeToUse) const
{
    hvalue result;
    hydro(source, result, scopeToUse, runtimeToUse);
    return result;
}

bool RunService::hydro(Source *source, hvalue &result, MemorySpace *scopeToUse, Runtime *runtimeToUse) const
{
    if (!runtimeToUse)
        runtimeToUse = _env->appDomain()->system();
    
    // init scope
    if (!scopeToUse)
        scopeToUse = new MemorySpace{_env->globals()};

    // create runtime
    Runtime *runtime = runtimeToUse ? runtimeToUse : new Runtime{_env->appDomain()->system()};
    
    // compiler settings
    HydroCompilerSettings *settings = new HydroCompilerSettings{};
    settings->srcPath = _srcPath;
    settings->sdkPath = _sdkPath;
    settings->libPath = _libPath;
    
    // create compiler
    HydroCompiler *compiler = new HydroCompiler{settings, _env, runtime};
    compiler->appendLibrary(_env->nativeLib());
    delete settings;
    compiler->debug(_debug);
    
    compiler->compile(source, h3o_compile_eval);
    
    if (compiler->failed())
    {
        // compilation error
        std::string text = compiler->errorTextAt(0);
        std::string src = compiler->errorSourceAt(0);
        uint32_t line = compiler->errorLineAt(0);
        delete compiler;
        ParseError *err = new ParseError{text, src, line};
        throw err;
    }

    Chunk *program = compiler->result();
    delete compiler;
    return _vm->exec(_currentContext, program, result, scopeToUse);
    /*
    try
    {
        return _vm->exec(_currentContext, program, result, scopeToUse);
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
    return false;*/
}


bool RunService::hydroFile(std::string filepath, hvalue &result, MemorySpace *scopeToUse, Runtime *runtimeToUse) const
{
    FileSource *source = new FileSource{filepath};
    
    // try open file stream
    source->open();
    
    if (!source->isOpen())
        throw std::runtime_error{"Failed to open file source \"" + filepath + "\"."};
    
    return hydro(source, result, scopeToUse, runtimeToUse);
}

hvalue RunService::hydroFile(std::string filepath, MemorySpace *scopeToUse, Runtime *runtimeToUse) const
{
    hvalue result;
    hydroFile(filepath, result, scopeToUse, runtimeToUse);
    return result;
}

bool RunService::hydroScript(std::string hydroCode, std::string filepath, hvalue &result, MemorySpace *scopeToUse, Runtime *runtimeToUse) const
{
    StringSource *source = new StringSource{hydroCode, filepath};
    return hydro(source, result, scopeToUse, runtimeToUse);
}

hvalue RunService::hydroScript(std::string hydroCode, std::string filepath, MemorySpace *scopeToUse, Runtime *runtimeToUse) const
{
    hvalue result;
    hydroScript(hydroCode, filepath, result, scopeToUse, runtimeToUse);
    return result;
}

} // namespace hydro
