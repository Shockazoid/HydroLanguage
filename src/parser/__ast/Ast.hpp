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

#ifndef __h3o_Ast__
#define __h3o_Ast__

#include "../Token.hpp"
#include "../../pattern/vpVisitor.hpp"

namespace hydro
{

class Scope;
class Symbol;
class Modifier;

typedef class Ast *ast_node;

class Ast : public vpElement
{
protected:
	lex_token _token{};
	ast_node _parent;
	ast_node _front; // first child
	ast_node _back;  // last child
	ast_node _prev;
	ast_node _next;
	uint32_t _nchildren{0};
    Symbol *_symbol;
    
	Ast(lex_token token = nullptr, Symbol *symbol = nullptr);
    //Ast(Token *token = nullptr, Ast *parent = nullptr);
	void addChild(ast_node child);
	void removeChild(ast_node child);
    void bind(Symbol *symbol);
    
public:
    //Binding *binding{nullptr};
	virtual ~Ast();
	Ast *getChildAt(uint32_t index) const;
	void remove();
	void visitChildren(vp_visitor visitor);
	void visitChildren(vp_visitor visitor, uint32_t startIndex);
	lex_token token() const { return _token; }
	bool hasParent() const { return _parent != nullptr; }
	ast_node parent() const { return _parent; }
	ast_node prev() const { return _prev; }
	ast_node next() const { return _next; }
	ast_node firstChild() const { return _front; }
	ast_node lastChild() const { return _back; }
	bool isNil() const { return (_token != nullptr) || (_token->type() == nil_tkn); }
	uint32_t numChildren() const { return _nchildren; }
    bool hasChildren() const { return _front != nullptr; }
	std::string tokenValue() const { return _token ? _token->value() : ""; }
	int32_t tokenType() const { return _token ? _token->type() : nil_tkn; }
	int32_t index() const { return _token ? _token->getTokenIndex() : -1; }
	uint32_t line() const { return _token ? _token->line() : 0; }
	std::string source() const { return _token ? _token->source() : ""; }
	template <typename X>
	X *tryCast() { return dynamic_cast<X *>(this); }
    Symbol *symbol() const { return _symbol; }
    std::string nameOf() const;
    bool isAnonymous() const;
    Scope *ownScope() const;
};

} // namespace hydro

#endif /* __h3o_Ast__ */
