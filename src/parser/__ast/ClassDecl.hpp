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

#ifndef __h3o_ClassDecl__
#define __h3o_ClassDecl__

#include <list>
#include "TypeDecl.hpp"
#include "TypeReference.hpp"
#include "ClassSymbol.hpp"

namespace hydro
{

class ClassDecl : public TypeDecl
{
private:
    std::list<Name *> _types;

public:
    //ClassDecl(lex_token token, ClassSymbol *symbol, ast_stmt body, PackageSymbol *package = nullptr);
    ClassDecl(lex_token token, ast_mod mods, ast_name name, ast_stmt body, PackageSymbol *package = nullptr, ClassSymbol *symbol = nullptr);
	virtual ~ClassDecl();
    void appendType(Name *name) { _types.push_back(name); }
	const std::list<Name *> &types() const { return _types; }
};

typedef ClassDecl *ast_class;

} // namespace hydro

#endif /* __h3o_ClassDecl__ */
