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

#ifndef __h3o_ScopedDecl__
#define __h3o_ScopedDecl__

#include "Decl.hpp"
#include "PackageSymbol.hpp"

namespace hydro
{

class ScopedDecl : public Decl
{
private:
	ast_stmt _body;
	PackageSymbol * _package;

public:
    ScopedDecl(Token *token, Symbol *symbol, Stmt *body, PackageSymbol *package = nullptr);
    
	ScopedDecl(Token *token, Modifier *mod, Name *name, Stmt *body, PackageSymbol *package = nullptr, Symbol *symbol = nullptr);
	virtual ~ScopedDecl();
	ast_stmt body() const { return _body; }
	bool hasBody() const { return _body != nullptr; }
	PackageSymbol *package() const { return _package; }
};

} // namespace hydro

#endif /* __h3o_ScopedDecl__ */
