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

#include "HydroCompiler.hpp"
#include "../assembly/H3oAssembler.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

HydroCompiler::HydroCompiler(HydroCompilerSettings *settings, HvmEnv *env, Runtime *runtime) : Compiler{}, _env{env}, _assembler{new H3oAssembler{this, runtime}}, _runtime{runtime}, _mode{}, _debug{false}, _libPaths{}
{
    _sdkPath = settings->sdkPath;
    _srcPath = settings->srcPath;

    split(_libPaths, settings->libPath, '|');
    _globals = new SymbolTable{};
    _program = new Program{};
    _codeGen = new HydroCodeGenerator{this, _globals, _runtime};
}

HydroCompiler::~HydroCompiler()
{
	delete _codeGen;
	delete _assembler;
}

void HydroCompiler::appendLibrary(Library *lib)
{
    _libraries.push_back(lib);
}

Source *HydroCompiler::resolveSourceByQName(std::string qname)
{
    for(Library *lib : _libraries)
    {
        Source *src = lib->resolve(qname);
        
        if(src)
            return src; // success!
    }
    
    // fail
    return nullptr;
}

Source *HydroCompiler::resolveSourceByQName(std::string qname, uint32_t index)
{
    if(index < _libraries.size())
        return _libraries[index]->resolve(qname);
    
    // fail
    return nullptr;
}


void HydroCompiler::parseSource(Source *src)
{
    HydroParser *parser = new HydroParser{this, _program, _globals};
	parser->parse(src);
    delete parser;
}

void HydroCompiler::generateCode(hydro_compile_mode mode)
{
	_codeGen->generate(_program, _globals, mode);
}

void HydroCompiler::assembleBytecode()
{
	_assembler->program(_codeGen->assembly());
}

void HydroCompiler::compile(std::string hydroCode)
{
	compile(new StringSource{hydroCode, "(eval source)"});
}

void HydroCompiler::compile(std::string hydroCode, hydro_compile_mode mode)
{
	compile(new StringSource{hydroCode, "(eval source)"}, mode);
}

void HydroCompiler::compile(Source *src)
{
	compile(src, h3o_compile_aot);
}

void HydroCompiler::compile(Source *src, hydro_compile_mode mode)
{
	// initialize the mode
	_mode = mode;

	// init
	clear();

	// lexical and syntax analysis
	parseSource(src);

	if (failed())
		return; // abort

	// semantic analysis and code generation
	generateCode(mode);

	if (failed())
		return; // abort

	// assemble
	assembleBytecode();
}

bool HydroCompiler::compileFile(std::string path)
{
	FileSource *src = new FileSource{path};
	src->open();

	if (src->isOpen())
	{
		compile(src);
		return true; // successfully opened file
	}

	// io error
	return false;
}

bool HydroCompiler::compileFile(std::string path, hydro_compile_mode mode)
{
	FileSource *src = new FileSource{path};
	src->open();

	if (src->isOpen())
	{
		compile(src, mode);
		return true; // successfully opened file
	}

	// io error
	return false;
}

Chunk *HydroCompiler::result() const { return _assembler->result(); }

} // namespace hydro
