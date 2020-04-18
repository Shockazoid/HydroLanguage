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

#ifndef __h3o_Decl__
#define __h3o_Decl__

#include "Stmt.hpp"
#include "Symbol.hpp"

namespace hydro
{

class Decl : public Stmt
{
private:
    Symbol *_symbol;
    Modifier *_mods;
    Name *_name;
    
public:
    Decl(Token *token, Symbol *symbol);
	Decl(Token * token, ast_mod mods, ast_name name = nullptr, Symbol *symbol = nullptr);
	virtual ~Decl();
    Modifier *modifier() const { return _mods; }
    class Name *name() const { return _name; }
	virtual bool isUntyped() const { return true; }
};

typedef Decl *ast_decl;

} // namespace hydro

#endif /* __h3o_Decl__ */
