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

#ifndef __h3o_Lexer__
#define __h3o_Lexer__

#include <map>
#include <vector>
#include <regex>

#include "Token.hpp"
#include "Compiler.hpp"

namespace hydro
{

const std::regex lexer_identifier{"^[a-zA-Z_][a-zA-Z0-9_]*"};

const std::regex lexer_linefeed{"^[\\n]+$"};

const std::regex lexer_whitespace{"^[\\s\\t\\r]+$"};

const std::regex lexer_single_line_comment{"^\\/\\/[^\n]*$"};

const std::regex lexer_alpha{"^[a-zA-Z]+"};

const std::regex lexer_letter{"^[a-zA-Z]$"};

const std::regex lexer_alphanum{"^[a-zA-Z0-9]+"};

const std::regex lexer_alphanum_underscore{"^[a-zA-Z0-9_]+$"};

const std::regex lexer_digit{"^[\\d]+$"};

const std::regex lexer_number{"^(([0-9]+(([dDfFrR])|([uU]?[lLsS])))|([1-9][0-9]*([eE](\\+|\\-)?[0-9]+)?)|(0)|(0[0-7]+([eE](\\+|\\-)?[0-9]+)?)|(0[bB][0-1]+)|(0[xX][a-fA-F0-9]+)|(([0-9]+\\.[0-9]+)([eE](\\+|\\-)?[0-9]+)?([dDfFrR])?))$"};

const std::regex lexer_double{"^[0-9]+\\.[0-9]+([eE][\\+\\-]?[0-9]+)?$"};

const std::regex lexer_int{"^[1-9][0-9]*([eE](\\+|\\-)?[0-9]+)?(([dDfFrR])|([uU]?[lLsS]))?$"};

const std::regex lexer_octal{"^0[0-7]+$"};

const std::regex lexer_octal_digit{"^[0-7]$"};

const std::regex lexer_binary{"^0[bB][0-1]*$"};

const std::regex lexer_binary_digit{"^[0-1]$"};

const std::regex lexer_hexadecimal{"^0[xX][0-9a-fA-F]+$"};

const std::regex lexer_hexadecimal_digit{"^[0-9a-fA-F]$"};

class Lexer
{
private:
	struct TokenInfo
	{
		std::string name;
		std::string value;
		int32_t type;
		TokenInfo(std::string n, std::string v) : name{n}, value{v}, type{nil_tkn} {}
		TokenInfo(std::string n, int32_t t) : name{n}, value{}, type{t} {}
		~TokenInfo() {}
		bool compare(lex_token token) const { return token->type() == token->type(); }
		bool compare(std::string tokenValue) const { return value == tokenValue; }
		bool compare(int32_t tokenType) const { return type == tokenType; }
	};

	struct TokenPattern
	{
		int32_t type;
		std::regex pattern;
		char sentinel; // terminating character
		TokenPattern(int32_t t, std::regex p, char s = '\n') : type{t}, pattern{p}, sentinel{s} {}
		~TokenPattern() {}
	};

	std::vector<TokenInfo *> _tokenNames;
	std::vector<TokenPattern *> _tokenPatterns;
	lex_token _currentToken;
	lex_token _eofToken;
	lex_source _source;
	std::string _buffer;
	std::string _specBuffer;
	int32_t _specPos;
	std::vector<int32_t> _markers;
	int32_t _tokenPos;
	int32_t _pos;
    uint32_t _line;
    char _currentChar;

protected:
	Compiler *_compiler;
    std::string _content; // loaded content

	Lexer(Compiler *compiler);

	// arbitrary lookahead
	char la(int32_t k);

	// buffering
	std::string buffer() const { return _buffer; }
    std::string bufferWithCurrent() { return _currentChar ? _buffer + _currentChar : ""; }
    std::string bufferWithNext() { return hasNext() ? _buffer + la(1) : ""; }
	lex_token flush(int32_t tokenType);

	// token information
	void registerTokenName(std::string tokenValue, std::string tokenName);
	void registerTokenName(int32_t tokenType, std::string tokenName);
	TokenInfo *findTokenName(std::string tokenValue) const;
	TokenInfo *findTokenName(int32_t tokenType) const;
	void registerTokenPattern(int32_t tokenType, std::string pattern);
	void registerTokenPattern(int32_t tokenType, std::string pattern, char sentinel);
	void registerTokenPattern(int32_t tokenType, std::regex pattern);
	void registerTokenPattern(int32_t tokenType, std::regex pattern, char sentinel);
	TokenPattern *findTokenPattern(int32_t tokenType) const;

	// errors
	void appendError(std::string errorTxt, uint32_t line, int32_t index);

	// eof helper
	bool isEof(int32_t k);

	// matching helpers
	bool matchLa(char ch, int32_t k);
	bool matchLa(const std::regex &pattern, int32_t k);
	bool matchLa(int32_t registeredTokenType, int32_t k);

	bool matchBuffer(const std::regex &pattern);
	bool matchBuffer(int32_t registeredTokenType);

	bool matchBufferWithCurrent(const std::regex &pattern);
	bool matchBufferWithCurrent(int32_t registeredTokenType);

	bool matchBufferWithNext(const std::regex &pattern);
	bool matchBufferWithNext(int32_t registeredTokenType);

	bool matchCurrent(char ch);
	bool matchCurrent(const std::regex &pattern);
	bool matchCurrent(int32_t registeredTokenType);

	bool matchPrev(char ch);
	bool matchPrev(const std::regex &pattern);
	bool matchPrev(int32_t registeredTokenType);

	bool matchNext(char ch);
	bool matchNext(const std::regex &pattern);
	bool matchNext(int32_t registeredTokenType);

	char current() { return la(0); }
	char prev() { return la(-1); }
	char next() { return la(1); }

	bool currentExists();
	bool prevExists();
	bool nextExists();

	lex_token devour(int32_t registeredTokenType);

	// speculation
	void specStart();
	bool specSuccess();
	bool specFail();
	void mark();
	void release();
	void seek(int32_t k);
	int32_t specPos() const;
	int32_t specUp();
	int32_t specDown();
	bool specMatch(char ch);
	bool specMatch(const std::regex &pattern);
	bool specMatch(int32_t registeredTokenType);
	bool specExists();
	char specCurrent();
	char specPrev();
	char specNext();
	bool isSpeculating() const { return !_markers.empty(); }

	// lexer operations
    char readChar();
	void advance();
	void consume();

	// skip helpers
	void skip();
	void skipOnce(char ch);
	void skipAll(char ch);
	void skipOnce(const std::regex &pattern);
	void skipAll(const std::regex &pattern);
	void abrupt();

	// reset
	virtual void unload();

	// abstract methods
	virtual lex_token readNext() = 0;

public:
	bool trace;

	virtual ~Lexer();
	void load(Source *src);
	bool hasNext() const;
	lex_token nextToken();
	lex_token nextToken(int32_t pos);
	lex_token nextChar();
	lex_token nextChar(int32_t pos);
	lex_token charAt(int32_t index);
	lex_token lookChar(int32_t k);
	void jump();
	lex_token tokenize();
	void dump();
	std::string getTokenName(lex_token token);
	std::string getTokenName(std::string tokenValue);
	std::string getTokenName(int tokenType);
	Compiler *compiler() const { return _compiler; }
	lex_token currentToken() const { return _currentToken; }
	void currentToken(lex_token token) { _currentToken = token; }
	lex_source source() const { return _source; }
    int32_t position() const { return _pos; }
    uint32_t line() const { return _line; }
};

typedef Lexer *lex_lexer;

} // namespace hydro

#endif /* __h3o_Lexer__ */
