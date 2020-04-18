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

#ifndef __h3o_Parser__
#define __h3o_Parser__

#include <string>
#include <vector>
#include <list>
#include <regex>
#include <stack>

#include "Lexer.hpp"
#include "__ast/Ast.hpp"
#include "__ast/Scope.hpp"

namespace hydro
{

enum binary_assoc
{
	op_left_assoc,
	op_right_assoc
};

class Parser
{
private:
	struct bad_parse_abort
	{
	};

	lex_token _tokens; // token tree
	lex_token _currentToken;
	lex_token _currentChar;
	std::vector<int32_t> _markers;
	std::vector<int32_t> _ignoredTokenTypes;
	std::list<lex_token> _lookahead; // buffer of lookahead tokens
	bool laCharActive{false};
	int32_t _index; // current token index (TODO remove)
	int32_t _specPos;
	void __parse();

protected:
	Compiler *_compiler;
	Lexer *_lexer;
	void *_result;
	ast_scope _currentScope;
	ast_scope _symbolTable;
	bool _parsing;

	Parser(Lexer *lexer);

	// parser methods
	void abort();
	virtual void reset();
	void insert(lex_token token);
	lex_token laPopFront();
	void pushLaToken(lex_token token);
	void advance();
	void advanceChar();
	//void next();	     // legacy
	//void next(unsigned k); // legacy
	//void nextChar();	 // legacy
	void consume();
	void consume(std::string tokenValue);
	void consume(int32_t tokenType);
	void consume(const std::regex &pattern);
	void abrupt();
	void abrupt(lex_token erroneousToken);
	void appendError(std::string errorText, lex_token erroneousToken = nullptr);
	void skip();
	void skip(std::string tokenValue);
	void skip(int32_t tokenType);
	void skip(const std::regex &pattern);
	void advanceSkip(std::string tokenValue);
	void advanceSkip(int32_t tokenType);
	void advanceSkip(const std::regex &pattern);
	void nextSkip(std::string tokenValue);    // legacy
	void nextSkip(int32_t tokenType);		// legacy
	void nextSkip(const std::regex &pattern); // legacy
	std::string tokenVal();				// legacy
	int32_t tokenType();				// legacy
	std::string nextVal();				// legacy
	int32_t nextType();				// legacy

	// arbitrary lookahead
	lex_token la(int32_t k);
	lex_token laChar(int32_t k); // by character
	std::string laVal(int32_t k);
	int32_t laType(int32_t k);
	bool tokenExists();     // legacy
	bool nextTokenExists(); // legacy
	bool isEof();		// legacy
	void clearBuffer() { _lookahead.clear(); }

	// skipping tokens
	void ignore(int32_t tokenType);

	// matching
	bool match(std::string tokenValue, lex_token token);
	bool match(std::string tokenValue, int32_t k);
	bool match(std::string tokenValue);
	bool match(int32_t tokenType, lex_token token);
	bool match(int32_t tokenType, int32_t k);
	bool match(int32_t tokenType);
	bool match(const std::regex &pattern, std::string value);
	bool match(const std::regex &pattern, lex_token token);
	bool match(const std::regex &pattern, int32_t k);
	bool match(const std::regex &pattern);
	bool match(char ch, lex_token token);
	bool match(char ch, int32_t k);
	bool match(char ch);
	bool matchNext(std::string tokenValue);
	bool matchNext(int32_t tokenType);
	bool matchNext(const std::regex &pattern);
	bool matchNext(std::string tokenValue, std::string skipTokenValue);
	bool matchNext(char ch);
	bool matchNext(int32_t tokenType, std::string skipTokenValue);
	bool matchNext(const std::regex &pattern, std::string skipTokenValue);
	bool matchNext(std::string tokenValue, int32_t skipTokenType);
	bool matchNext(int32_t tokenType, int32_t skipTokenType);
	bool matchNext(const std::regex &pattern, int32_t skipTokenType);
	bool matchNext(std::string tokenValue, const std::regex &skipTokenPattern);
	bool matchNext(int32_t tokenType, const std::regex &skipTokenPattern);
	bool matchNext(const std::regex &pattern, const std::regex &skipTokenPattern);

	// validation
	void validate(std::string tokenValue);
	void validate(int32_t tokenType);
	void validate(const std::regex &pattern);

	// backtracking and pack rat
	void seek(int32_t k);
	int32_t mark();
	void release();
	void memoize();
	virtual void clearMemo();
	void specStart();
	bool specSuccess();
	bool specFail();
	bool specMatch(std::string tokenValue);
	bool specMatch(int32_t tokenType);
	bool specMatch(const std::regex &pattern);
	bool specMatch(std::string tokenValue, std::string skipTokenValue);
	bool specMatch(std::string tokenValue, int32_t skipTokenType);
	bool specMatch(std::string tokenValue, const std::regex &skipPattern);
	bool specMatch(char ch);
	bool specExists();
	void specUp();
	void specDown();
	std::string specVal();
	int32_t specType();
	lex_token specSkip(std::string tokenValue);
	lex_token specSkip(int32_t tokenType);
	lex_token specSkip(const std::regex &pattern);
	int32_t specPos() const { return _specPos; }

	// symbol table
	void define(ast_symbol symbol);
	ast_symbol resolve(std::string name, ast_scope scopeToUse = nullptr);
	ast_scope pushScope(ast_scope scope);
	ast_scope popScope();

	// abstract methods
	virtual void onParseInit();
	virtual void onParseComplete();
	virtual void onParseFailure();
	virtual void parseNext() = 0;

public:
	virtual ~Parser();
	virtual void parse(Source *src);
	bool willIgnore(lex_token token) const;
	bool willIgnore(int32_t tokenType) const;
	Compiler *compiler() const { return _compiler; }
	lex_lexer lexer() const { return _lexer; }
	lex_token currentToken() const { return _currentToken; }
	void *result() const { return _result; }
	bool isSpeculating() const { return !_markers.empty(); }
	int32_t currentTokenIndex() const { return _index; }
	ast_scope currentScope() const { return _currentScope; }
	ast_scope symbolTable() const { return _symbolTable; }
};

typedef Parser *ast_parser;

} // namespace hydro

#endif /* __h3o_Parser__ */
