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

#ifndef __h3o_HydroCompiler__
#define __h3o_HydroCompiler__

#include "Compiler.hpp"
#include "HydroCompilerSettings.hpp"
#include "HydroCodeGenerator.hpp"
#include "hydro_compile_mode.hpp"
#include "FileSource.hpp"
#include "StringSource.hpp"
#include "__ast/Program.hpp"
#include "../vm/Chunk.hpp"
#include "../utility/stringhelper.hpp"
#include "../library/Library.hpp"

namespace hydro
{

class HydroCompiler : public Compiler
{
    friend class HydroVisitor;
    friend class HydroAnalyzer;
    friend class HydroCodeGenerator;

private:
	struct HvmEnv *_env;
	HydroCodeGenerator *_codeGen;
	class H3oAssembler *_assembler;
	Runtime *_runtime;
	hydro_compile_mode _mode;
    Scope *_globals;
    Program *_program;
	bool _debug;
	std::string _sdkPath;
    std::string _srcPath;
    std::vector<std::string> _libPaths;
    std::vector<Library *> _libraries;

	void parseSource(Source *src);
	void generateCode(hydro_compile_mode mode);
	void assembleBytecode();

public:
	HydroCompiler(HydroCompilerSettings *settings, HvmEnv *env, Runtime *runtime);
	virtual ~HydroCompiler();
    void appendLibrary(Library *lib);
    Source *resolveSourceByQName(std::string qname);
    Source *resolveSourceByQName(std::string qname, uint32_t index);
	virtual void compile(std::string hydroCode) override;
	void compile(std::string hydroCode, hydro_compile_mode mode);
	virtual void compile(Source *src) override;
	virtual void compile(Source *src, hydro_compile_mode mode);
	virtual bool compileFile(std::string path) override;
	virtual bool compileFile(std::string path, hydro_compile_mode mode);
    std::string srcPath() const { return _srcPath; }
    std::string sdkPath() const { return _sdkPath; }
    std::string *libPaths() const
    {
        std::string *paths = new std::string[_libPaths.size() + 1];
        auto size = _libPaths.size();
        int i = 0;
        for(; i < size; i++)
        {
            paths[i] = _libPaths[i];
        }
        paths[i] = ""; // last element is empty string
        return paths;
    }
    Chunk *result() const;
	Runtime *runtime() const { return _runtime; }
	hydro_compile_mode mode() const { return _mode; }
	HvmEnv *env() const { return _env; }
	bool debug() const { return _debug; }
    void debug(bool value) { _debug = value; }
    uint32_t nlibs() const { return (uint32_t)_libraries.size(); }
};

} // namespace hydro

#endif /* __h3o_HydroCompiler__ */
