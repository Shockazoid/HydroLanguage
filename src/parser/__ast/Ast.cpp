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

#include "Ast.hpp"
#include "Symbol.hpp"
#include "Scope.hpp"

namespace hydro
{

Ast::Ast(lex_token token, Symbol *symbol) : _token{token}, _front{nullptr}, _back{nullptr}, _parent{nullptr}, _prev{nullptr}, _next{nullptr}, _symbol{nullptr} { bind(symbol); }

Ast::~Ast()
{
	// free node tree
	delete _front;
	delete _next;
}

void Ast::bind(Symbol *symbol)
{
    if(!symbol)
        return;
    if(_symbol)
        _symbol->_owner = nullptr; // unbind
    _symbol = symbol;
    _symbol->_owner = this;
}

void Ast::addChild(ast_node child)
{
	if (child == nullptr)
		return; // ignore

	if (child->_parent)
		child->_parent->removeChild(child);

	if (_back)
		_back->_next = child;

	child->_parent = this;
	child->_prev = _back;
	child->_next = nullptr;
	_back = child; // push back
	_nchildren++;

	if (!_front)
		_front = child;
}

void Ast::removeChild(ast_node child)
{
	if (child->_parent == this)
	{
		ast_node nxt = child->_next;
		ast_node prv = child->_prev;

		child->_next = child->_prev = nullptr;

		if (prv)
			prv->_next = nxt;

		if (nxt)
			nxt->_prev = prv;

		child->_front = nullptr;
		child->_parent = nullptr;
	}
}

Ast *Ast::getChildAt(uint32_t index) const
{
	Ast *child = firstChild();

	for (uint32_t i = 0; child && i < index; i++)
		if (child)
			child = child->next();

	return child;
}

void Ast::remove()
{
	if (_parent)
		_parent->removeChild(this);
}

void Ast::visitChildren(vp_visitor visitor)
{
	visitChildren(visitor, 0);
}

void Ast::visitChildren(vp_visitor visitor, uint32_t startIndex)
{
	ast_node node = _front;
	uint32_t k = 0;

	while (node)
	{
		if (k++ >= startIndex)
			node->accept(visitor);

		node = node->_next;
	}
}

std::string Ast::nameOf() const { return _symbol ? _symbol->nameOf() : ""; }
   
bool Ast::isAnonymous() const { return _symbol && (_symbol->name() == nullptr || _symbol->nameOf().empty()); }
   
Scope *Ast::ownScope() const { return _symbol ? _symbol->ownScope() : nullptr; }

} // namespace hydro
