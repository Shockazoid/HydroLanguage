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

#ifndef __h3o_Scope__
#define __h3o_Scope__

#include <string>
#include <list>
#include <cstdlib>

#include "Symbol.hpp"

namespace hydro
{

typedef class Scope *ast_scope;

class Scope
{
    friend Symbol;
    
private:
	std::string _name;
	ast_scope _parent;
    Symbol *_symbol;
	std::list<ast_symbol> _symbols;
    
    bool isAncestorOf(ast_scope child) const;
    
public:
	uint32_t upvalues{0};
	Scope(ast_scope parent = nullptr);
	Scope(std::string name, ast_scope parent = nullptr);
	virtual ~Scope();
	void define(ast_symbol symbol);
	void adopt(ast_scope child);
	bool contains(ast_symbol symbol) const;
	bool exists(std::string name) const;
	Symbol *get(std::string name);
	virtual Symbol *resolve(std::string name);
	std::string getName() const { return _name; }
	ast_scope enclosingScope() const { return _parent; }
	ast_scope parent() const { return _parent; } // alias
	bool isAnonymous() const { return _name == ""; }
	int32_t indexOf(ast_symbol symbol);
    Symbol *owner() const;
};

} // namespace hydro

#endif /* __h3o_Scope__ */
