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

#ifndef __h3o_Symbol__
#define __h3o_Symbol__

#include <string>

#include "SimpleName.hpp"
#include "Ast.hpp"
#include "Modifier.hpp"

namespace hydro
{

class Scope;

class Symbol
{
	friend Scope;
	friend Ast;

private:
    Modifier *_modifier;
    Name *_name;
    Ast *_owner;
    Symbol *_parent;
    class Scope *_ownScope;
    Scope *_enclosingScope;
    
public:
    int32_t slot{-1};
	Symbol(Modifier *modifier = nullptr, Name *name = nullptr, Scope *ownScope = nullptr, Symbol *parent = nullptr);
	virtual ~Symbol();
    bool hasMod(std::string modifier) const;
    Modifier *modifier() const { return _modifier; }
    bool isAnonymous() const { return _name == nullptr; }
    Name *name() const { return _name; }
    std::string nameOf() const { return _name ? _name->value() : ""; }
    bool hasOwner() const { return _owner != nullptr; }
    void owner(Ast *value) { _owner = value; }
    Ast *owner() const { return _owner; }
    bool hasOwnScope() const { return _ownScope != nullptr; }
    Scope *ownScope() const { return _ownScope; }
    bool isDefined() const { return _enclosingScope != nullptr; }
    bool hasEnclosingScope() const { return isDefined(); } // alias
    Scope *enclosingScope() const { return _enclosingScope; }
    bool isBound() const { return _owner != nullptr; }
    lex_token token() const { return _owner ? _owner->token() : nullptr; }
    Symbol *parent() const { return _parent; }
    int32_t index();
};

typedef Symbol *ast_symbol;

} // namespace hydro

#endif /* __h3o_Symbol__ */
