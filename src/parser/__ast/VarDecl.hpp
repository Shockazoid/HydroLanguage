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

#ifndef __h3o_VarDecl__
#define __h3o_VarDecl__

#include "Decl.hpp"
#include "TypeSpec.hpp"
#include "Expr.hpp"
#include "VarSymbol.hpp"

namespace hydro
{

class VarDecl : public Decl
{
private:
	ast_htype _type;
	ast_expr _defaultValue;

public:
	VarDecl(lex_token token, ast_mod mods, ast_name name, ast_htype type, ast_expr defaultValue = nullptr, Symbol *symbol = nullptr);
	virtual ~VarDecl();
	ast_htype type() const { return _type; }
	virtual bool isUntyped() { return _type == nullptr || _type->tryCast<NilSpec>(); }
	bool hasDefaultValue() const { return _defaultValue != nullptr; }
	ast_expr defaultValue() const { return _defaultValue; }
};

typedef VarDecl *ast_var;

} // namespace hydro

#endif /* __h3o_VariableBinding__ */
