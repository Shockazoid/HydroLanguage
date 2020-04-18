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

#include "Token.hpp"

namespace hydro
{

Token::Token(lex_source source, std::string value, int32_t type, uint32_t lineNum, int32_t pos) : _src{source->location()}, _value{value}, _type{type}, _lineNum{lineNum}, _pos{pos}, _prevToken{nullptr}, _nextToken{nullptr} {}

Token::Token(std::string value, int32_t type, uint32_t lineNum, int32_t pos) : _src{}, _value{value}, _type{type}, _lineNum{lineNum}, _pos{-1}, _prevToken{nullptr}, _nextToken{nullptr} {}

Token::~Token()
{
	//delete _nextToken;
}

void Token::insert(lex_token token)
{
	if (token == this)
		return;
	token->remove();
	token->_prevToken = this;
	token->_nextToken = _nextToken;
	_nextToken = token;
}

void Token::remove()
{
	// remove from end of tree
	if (_nextToken)
		_nextToken->_prevToken = _prevToken;

	// remove from start of tree
	if (_prevToken)
		_prevToken->_nextToken = _nextToken;
}

void Token::dispose()
{
	remove();
	delete this;
}

std::string Token::source() const { return _src; }

uint32_t Token::line() const { return _lineNum; }

int32_t Token::position() const { return _pos; }

std::string Token::value() const { return _value; }

int32_t Token::type() const { return _type; }

lex_token Token::prev() const { return _prevToken; }

lex_token Token::next() const { return _nextToken; }

lex_token Token::begin()
{
	lex_token tkn = this;

	while (tkn->_prevToken)
		tkn = tkn->_prevToken;

	return tkn;
}

lex_token Token::end()
{
	lex_token tkn = this;

	while (tkn->_nextToken)
		tkn = tkn->_nextToken;

	return tkn;
}

lex_token Token::siblingAt(int32_t k)
{
	return nullptr;
}

uint32_t Token::getTokenIndex()
{
	uint32_t index = 0;
	lex_token tkn = this;

	while (tkn->_prevToken)
	{
		tkn = tkn->_prevToken;
		index++;
	}

	return index;
}

void Token::dump()
{
	std::cout << "Token(" << _value << ":" << _type << ")" << std::endl;
}

} // namespace hydro
