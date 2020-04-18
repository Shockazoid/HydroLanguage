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

#ifndef __h3o_PropertyDecl__
#define __h3o_PropertyDecl__

#include "Decl.hpp"
#include "Name.hpp"
#include "Expr.hpp"
#include "TypeSpec.hpp"
#include "Modifier.hpp"
#include "PropertySymbol.hpp"

namespace hydro
{

class PropertyDecl : public Decl
{
private:
	ast_htype _type;
	ast_expr _defaultValue;
	Stmt *_body;

public:
	PropertyDecl(lex_token token, ast_mod mods, ast_name name, ast_htype type, ast_expr defaultValue = nullptr, PropertySymbol *symbol = nullptr);
	PropertyDecl(lex_token token, ast_mod mods, ast_name name, ast_htype type, Stmt *body, PropertySymbol *symbol = nullptr);
	virtual ~PropertyDecl();
	ast_htype type() const { return _type; }
	bool hasDefaultValue() const { return _defaultValue != nullptr; }
	ast_expr defaultValue() const { return _defaultValue; }
	bool hasBody() const { return _body != nullptr; }
	Stmt *body() const { return _body; }
};

typedef PropertyDecl *ast_property;

} // namespace hydro

#endif /* __h3o_PropertyDecl__ */
