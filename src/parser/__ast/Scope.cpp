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

#include "Scope.hpp"

namespace hydro
{

Scope::Scope(ast_scope parent) : _name{""}, _parent{parent}, _symbols{}, _symbol{nullptr}
{
}

Scope::Scope(std::string name, ast_scope parent) : _name{name}, _parent{parent}, _symbols{}, _symbol{nullptr}
{
}

Scope::~Scope()
{
	while (!_symbols.empty())
	{
		delete _symbols.back();
		_symbols.pop_back();
	}
}


bool Scope::isAncestorOf(ast_scope child) const
{
    ast_scope s = child->_parent;

    while (s)
    {
        if (s == this)
            return true;

        s = s->_parent;
    }

    return false;
}

void Scope::adopt(ast_scope child)
{
    if(child->_parent)
        return;
    else if(child == this)
        return; // cannot be a child of itself
	else if (child->isAncestorOf(this))
		return; // cannot be a parent of ancestor
	child->_parent = this;
}

void Scope::define(ast_symbol symbol)
{
	symbol->_enclosingScope = this;
	_symbols.push_back(symbol);
}

bool Scope::contains(ast_symbol symbol) const
{
    for (ast_symbol sym : _symbols)
        if (sym == symbol)
            return true;

    // fail
    return false;
}

bool Scope::exists(std::string name) const
{
	for (ast_symbol sym : _symbols)
		if (!sym->isAnonymous() && sym->nameOf() == name)
			return true;

    // fail
	return false;
}

Symbol *Scope::get(std::string name)
{
	for (ast_symbol sym : _symbols)
		if (!sym->isAnonymous() && sym->nameOf() == name)
			return sym;

	// fail
	return nullptr;
}

Symbol *Scope::resolve(std::string name)
{
	// search
	if (ast_symbol symbol = get(name))
		return symbol;

	// try again
	if (_parent)
		return _parent->resolve(name);

	// fail
	return nullptr;
}

int32_t Scope::indexOf(ast_symbol symbol)
{
	int32_t i = 0;

	for (ast_symbol sym : _symbols)
	{
		if (sym == symbol)
			return i; // success!
        i++;
	}

	// fail
	return -1;
}

Symbol *Scope::owner() const
{
    return _symbol;
}

} // namespace hydro
