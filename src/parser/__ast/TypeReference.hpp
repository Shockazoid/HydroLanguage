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

#ifndef __h3o_TypeReference__
#define __h3o_TypeReference__

#include "Name.hpp"
#include "PackageSpec.hpp"

namespace hydro
{

class TypeReference : public Ast
{
private:
	ast_package _pkg;
	ast_name _name;

public:
	TypeReference(ast_name name);
	TypeReference(ast_package pkg, lex_token nqToken, ast_name name);
	virtual ~TypeReference();
	bool hasPackage() const { return _pkg != nullptr; }
	ast_package package() const { return _pkg; }
	ast_name name() const { return _name; }
};

typedef TypeReference *ast_typeref;

} // namespace hydro

#endif /* __h3o_TypeReference__ */
