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

#include "Parser.hpp"

namespace hydro
{

Parser::Parser(Lexer *lexer) : _lexer{lexer}, _tokens{nullptr}, _currentToken{nullptr}, _currentScope{nullptr}, _markers{}, _ignoredTokenTypes{}, _lookahead{}, _specPos{0}, _index{-1}, _parsing{false}
{
	_compiler = lexer->compiler();
}

Parser::~Parser()
{
	//if (_currentToken)
		//delete _currentToken->top();
}

void Parser::__parse()
{
	if (_parsing)
		return;

	_parsing = true;

	while (_parsing && _lexer->hasNext())
	{
		advance();
		if (isEof())
			break; // end of file

		parseNext();
	}

	_parsing = false;
}

void Parser::abort()
{
	_parsing = false;
	throw bad_parse_abort{};
}

void Parser::reset()
{
	_parsing = false;
	_currentScope = nullptr;
	_tokens = _currentToken = nullptr; // clear token pointer and token tree
	_markers.clear();
	_index = -1;
	_specPos = 0;
	_lookahead.clear();
}

void Parser::insert(lex_token token)
{
	if (!token)
		return;

	// insert token into token tree or create tree
	if (_tokens)
		_tokens->insert(token);

	_tokens = token;
}

lex_token Parser::laPopFront()
{
	if (_lookahead.empty())
		return nullptr; // empty
	lex_token front = _lookahead.front();
	_lookahead.pop_front();
	return front;
}

void Parser::pushLaToken(lex_token token)
{
	_lookahead.push_back(token);
}
/*
void Parser::advance()
{
	_markers.clear();
	_specPos = 0;
	clearMemo();
    
	if (_currentToken && _currentToken->next())
	{
		// lookahead token exists
		do
        {
			_currentToken = _currentToken->next();
            insert(_currentToken);
        }
		while (_currentToken && willIgnore(_currentToken->type()));
        
	}
	else if (_lexer->hasNext())
	{
		do
		{
			// load token
			_currentToken = _lexer->nextToken();
            //insert(_currentToken);
			// _currentToken->dump();
		} while (_currentToken && willIgnore(_currentToken->type()));
	}
}*/

void Parser::advance()
{
	// reset speculation
	_markers.clear();
	_specPos = 0;
	clearMemo();

	// insert current token into token tree (if exists)
	if (_currentToken)
		insert(_currentToken);

	// advance to next recognizable token
	lex_token token;

	do
	{
		// try to get lookahead token
		token = laPopFront();

		if (token)
		{
			// use lookahead token
			_currentToken = token;
		}
		else
		{
			// read next token
			_currentToken = _lexer->nextToken();
		}
	} while (willIgnore(_currentToken));
}

void Parser::advanceChar()
{
	_markers.clear();
	_specPos = 0;
	clearMemo();

	// insert current token into token tree (if exists)
	//if(_currentToken)
	//    insert(_currentToken);

	// advance to next recognizable token
	// read next char
	_currentToken = _lexer->nextChar();
}

void Parser::consume()
{
	advance(); // alias
}

void Parser::consume(std::string tokenValue)
{
	validate(tokenValue);
	advance();
}

void Parser::consume(int32_t tokenType)
{
	validate(tokenType);
	advance();
}

void Parser::consume(const std::regex &pattern)
{
	validate(pattern);
	advance();
}

void Parser::abrupt()
{
	std::string val = _currentToken ? " (" + _currentToken->value() + ")" : "";

	// error
	appendError("Encountered unexpected " + _lexer->getTokenName(_currentToken) + ".", _currentToken);
	abort();
}

void Parser::abrupt(lex_token erroneousToken)
{
	std::string name;
	if (!erroneousToken)
		name = "nil token";
	else
		name = _lexer->getTokenName(erroneousToken);

	appendError("Encountered unexpected token " + name + ".", erroneousToken);
	abort();
}

void Parser::appendError(std::string errorTxt, lex_token erroneousToken)
{
	_compiler->appendError(errorTxt, erroneousToken);
}

void Parser::skip()
{
	consume();
}

void Parser::skip(std::string tokenValue)
{
	while (match(tokenValue))
		advance();
}

void Parser::skip(int32_t tokenType)
{
	while (match(tokenType))
		advance();
}

void Parser::skip(const std::regex &pattern)
{
	while (match(pattern))
		advance();
}

void Parser::advanceSkip(std::string tokenValue)
{
	advance(); // next
	skip(tokenValue);
}

void Parser::advanceSkip(int32_t tokenType)
{
	advance(); // next
	skip(tokenType);
}

void Parser::advanceSkip(const std::regex &pattern)
{
	advance(); // next
	skip(pattern);
}

void Parser::nextSkip(std::string tokenValue)
{
	advanceSkip(tokenValue); // alias
}

void Parser::nextSkip(int32_t tokenType)
{
	advanceSkip(tokenType);
}

void Parser::nextSkip(const std::regex &pattern)
{
	advanceSkip(pattern);
}

std::string Parser::tokenVal()
{
	return laVal(0); // current
}

int32_t Parser::tokenType()
{
	return laType(0); // current
}

std::string Parser::nextVal()
{
	return laVal(1); // next
}

int32_t Parser::nextType()
{
	return laType(1); // next
}

lex_token Parser::la(int32_t k)
{
	lex_token token = _currentToken;

	if (k < 0)
	{
		// find previous token
		token = _currentToken;

		while (token && k < 0)
		{
			token = token->prev(); // rewind

			if (token)
			{
				if (willIgnore(token->type()))
					continue; // skip

				k++;
			}
		}
	}
	else if (k > 0)
	{
		// find lookahead token la(k)
		if (k - 1 < _lookahead.size())
		{
			int32_t i = 0;

			// lookahead already exists at zero based index
			for (lex_token tkn : _lookahead)
				if (i++ == k - 1)
					return tkn;

			// fail
			return nullptr;
		}
		else
		{
			token = nullptr;				// default
			k = k - (int32_t)_lookahead.size(); // deduct previously loaded tokens

			// populate lookhead buffer until la(k)
			while (k > 0 && _lexer->hasNext())
			{
				token = _lexer->nextToken();

				if (willIgnore(token))
					continue; // skip

				_lookahead.push_back(token);
				k--;
			}
		}

		if (k > 0)
			token = nullptr; // fail
	}
	else
	{
		// k = 0
		return _currentToken;
	}

	return token;
}

lex_token Parser::laChar(int32_t k)
{
	return _lexer->lookChar(k);
}

std::string Parser::laVal(int32_t k)
{
	lex_token token = la(k);
	if (token)
		return token->value();
	return ""; // fail
}

int32_t Parser::laType(int32_t k)
{
	lex_token token = la(k);
	if (token)
		return token->type();
	return eof_tkn; // fail
}

bool Parser::tokenExists()
{
	return _currentToken != nullptr;
}

bool Parser::nextTokenExists()
{
	return la(1);
}

bool Parser::isEof()
{
	return _currentToken && _currentToken->type() == eof_tkn;
}

void Parser::ignore(int32_t tokenType)
{
	_ignoredTokenTypes.push_back(tokenType);
}

bool Parser::match(std::string tokenValue, lex_token token)
{
	return token && token->value() == tokenValue;
}

bool Parser::match(std::string tokenValue, int32_t k)
{
	return match(tokenValue, la(k));
}

bool Parser::match(std::string tokenValue)
{
	return match(tokenValue, _currentToken);
}

bool Parser::match(int32_t tokenType, lex_token token)
{
	return token && token->type() == tokenType;
}

bool Parser::match(int32_t tokenType, int32_t k)
{
	return match(tokenType, la(k));
}

bool Parser::match(int32_t tokenType)
{
	return match(tokenType, _currentToken);
}

bool Parser::match(const std::regex &pattern, std::string value)
{
	return std::regex_match(value, pattern);
}

bool Parser::match(const std::regex &pattern, lex_token token)
{
	return token && std::regex_match(token->value(), pattern);
}

bool Parser::match(const std::regex &pattern, int32_t k)
{
	return match(pattern, la(k));
}

bool Parser::match(const std::regex &pattern)
{
	return match(pattern, _currentToken);
}

bool Parser::match(char ch, lex_token token)
{
	return match(std::string{1, ch}, token);
}

bool Parser::match(char ch, int32_t k)
{
	return match(ch, laChar(k));
}

bool Parser::match(char ch)
{
	return match(ch, laChar(0));
}

bool Parser::matchNext(std::string tokenValue)
{
	return match(tokenValue, 1);
}

bool Parser::matchNext(int32_t tokenType)
{
	return match(tokenType, 1);
}

bool Parser::matchNext(const std::regex &pattern)
{
	return match(pattern, 1);
}

bool Parser::matchNext(char ch)
{
	return match(ch, 1);
}

bool Parser::matchNext(std::string tokenValue, std::string skipTokenValue)
{
	int32_t k = 1; // next token
	while (match(skipTokenValue, k))
		k++;
	return match(tokenValue, k);
}

bool Parser::matchNext(int32_t tokenType, std::string skipTokenValue)
{
	int32_t k = 1; // next token
	while (match(skipTokenValue, k))
		k++;
	return match(tokenType, k);
}

bool Parser::matchNext(const std::regex &pattern, std::string skipTokenValue)
{
	int32_t k = 1; // next token
	while (match(skipTokenValue, k))
		k++;
	return match(pattern, k);
}

bool Parser::matchNext(std::string tokenValue, int32_t skipTokenType)
{
	int32_t k = 1; // next token
	while (match(skipTokenType, k))
		k++;
	return match(tokenValue, k);
}

bool Parser::matchNext(int32_t tokenType, int32_t skipTokenType)
{
	int32_t k = 1; // next token
	while (match(skipTokenType, k))
		k++;
	return match(tokenType, k);
}

bool Parser::matchNext(const std::regex &pattern, int32_t skipTokenType)
{
	int32_t k = 1; // next token
	while (match(skipTokenType, k))
		k++;
	return match(pattern, k);
}

bool Parser::matchNext(std::string tokenValue, const std::regex &skipTokenPattern)
{
	int32_t k = 1; // next token
	while (match(skipTokenPattern, k))
		k++;
	return match(tokenValue, k);
}

bool Parser::matchNext(int32_t tokenType, const std::regex &skipTokenPattern)
{
	int32_t k = 1; // next token
	while (match(skipTokenPattern, k))
		k++;
	return match(tokenType, k);
}

bool Parser::matchNext(const std::regex &pattern, const std::regex &skipTokenPattern)
{
	int32_t k = 1; // next token
	while (match(pattern, k))
		k++;
	return match(pattern, k);
}

void Parser::validate(std::string tokenValue)
{
	if (!match(tokenValue))
	{
		appendError("Expecting " + tokenValue + " but received " + _lexer->getTokenName(_currentToken), _currentToken);
		abort();
	}
}

void Parser::validate(int32_t tokenType)
{
	if (!match(tokenType))
	{
		appendError("Expecting " + _lexer->getTokenName(tokenType) + " but received " + _lexer->getTokenName(_currentToken), _currentToken);
		abort();
	}
}

void Parser::validate(const std::regex &pattern)
{
	if (!match(pattern))
	{
		appendError("Encountered unexpected " + _lexer->getTokenName(_currentToken) + ".", _currentToken);
		abort();
	}
}

void Parser::seek(int32_t k)
{
	_specPos = k;
}

int32_t Parser::mark()
{
	_markers.push_back(_specPos);
	return _specPos;
}

void Parser::release()
{
	if (!_markers.empty())
	{
		int32_t m = _markers.back();
		_markers.pop_back();
		seek(m);
	}
}

void Parser::memoize()
{
}

void Parser::clearMemo()
{
	// abstract
}

void Parser::specStart()
{
	mark();
}

bool Parser::specSuccess()
{
	_markers.pop_back(); // don't restore position to original 'k'
	return true;
}

bool Parser::specFail()
{
	release(); // restores original 'k'
	return false;
}

bool Parser::specMatch(std::string tokenValue)
{
	specStart();

	if (match(tokenValue, _specPos))
	{
		_specPos++;
		return specSuccess();
	}

	return specFail();
}

bool Parser::specMatch(int32_t tokenType)
{
	specStart();

	if (match(tokenType, _specPos))
	{
		_specPos++;
		return specSuccess();
	}

	return specFail();
}

bool Parser::specMatch(const std::regex &pattern)
{
	specStart();

	if (match(pattern, _specPos))
	{
		_specPos++;
		return specSuccess();
	}

	return specFail();
}

bool Parser::specMatch(std::string tokenValue, std::string skipTokenValue)
{
	specSkip(skipTokenValue);
	return specMatch(tokenValue);
}

bool Parser::specMatch(std::string tokenValue, int32_t skipTokenType)
{
	specSkip(skipTokenType);
	return specMatch(tokenValue);
}

bool Parser::specMatch(std::string tokenValue, const std::regex &skipPattern)
{
	specSkip(skipPattern);
	return specMatch(tokenValue);
}

bool Parser::specMatch(char ch)
{
	specStart();

	if (match(ch, _specPos))
	{
		_specPos++;
		return specSuccess();
	}

	return specFail();
}

bool Parser::specExists()
{
	return la(_specPos) != nullptr;
}

void Parser::specUp()
{
	_specPos++;
}

void Parser::specDown()
{
	_specPos--;
}

std::string Parser::specVal()
{
	return laVal(_specPos);
}

int32_t Parser::specType()
{
	return laType(_specPos);
}

lex_token Parser::specSkip(std::string tokenValue)
{
	while (specMatch(tokenValue))
		;
	return la(_specPos);
}

lex_token Parser::specSkip(int32_t tokenType)
{
	while (specMatch(tokenType))
		;
	return la(_specPos);
}

lex_token Parser::specSkip(const std::regex &pattern)
{
	while (specMatch(pattern))
		;
	return la(_specPos);
}

void Parser::define(ast_symbol symbol)
{
	if (!_currentScope)
	{
		appendError("Attempted definition of symbol '" + symbol->name()->value() + "' outside of scope.", _currentToken);
		return;
	}
    
	_currentScope->define(symbol);
}

ast_symbol Parser::resolve(std::string name, ast_scope scopeToUse)
{
	if (!scopeToUse)
		scopeToUse = _currentScope;

	if (scopeToUse)
		return scopeToUse->resolve(name);

	return nullptr;
}

ast_scope Parser::pushScope(ast_scope scope)
{
	if (_currentScope)
		_currentScope->adopt(scope);

	_currentScope = scope;
	return scope;
}

ast_scope Parser::popScope()
{
	if (_currentScope)
	{
		ast_scope back = _currentScope->enclosingScope();
		_currentScope = back; // pop
		return back;
	}

	return nullptr;
}

void Parser::onParseInit()
{
	// abstract
}

void Parser::onParseComplete()
{
	// abstract
}

void Parser::onParseFailure()
{
	// abstract
}

void Parser::parse(Source *src)
{
	_lexer->load(src);
	reset();
	onParseInit();
	try
	{
		// parse until fatal error
		__parse();
	}
	catch (bad_parse_abort)
	{
		onParseFailure();
		return;
	}
	onParseComplete();
}

bool Parser::willIgnore(lex_token token) const
{
	return token ? willIgnore(token->type()) : false;
}

bool Parser::willIgnore(int32_t tokenType) const
{
	if (tokenType == eof_tkn)
		return false; // next ignore eof

	for (int32_t type : _ignoredTokenTypes)
		if (tokenType == type)
			return true;

	return false;
}

} // namespace hydro
