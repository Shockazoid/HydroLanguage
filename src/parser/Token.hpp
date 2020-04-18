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

#ifndef __h3o_Token__
#define __h3o_Token__

#include <string>

#include "Source.hpp"
#include "../memory/shared_ptr.hpp"

namespace hydro
{

enum std_token_type : int
{
	eof_tkn = -4,  // EOF
	nil_tkn = -3,  // nil
	ln_tkn = -2,   // linefeed
	ws_tkn = -1,   // whitespace
	token_tkn = 0, // standard token
	id_tkn = 1,    // identifier
	scomment_tkn,  // single-line comment
	mcomment_tkn,  // multi-line comment
	op_tkn,	  // operator
	char_tkn,	// character literal
	bool_tkn,	// boolean literal
	int_tkn,	 // integer literal
	uint_tkn,	// unsigned literal
	bin_tkn,	 // binary literal
	hex_tkn,	 // hexadecimal literal
	oct_tkn,	 // octal literal
	float_tkn,     // float literal
	double_tkn,    // double literal
	string_tkn,    // string literal
	template_tkn,  // string template
	xml_tkn,	 // xml literal
	regex_tkn,     // regular expression literal
	ushort_tkn,    // unsigned short literal
	short_tkn,     // short literal
	ulong_tkn,     // unsigned long literal
	long_tkn,	// long literal
	package_tkn,   // hydro package literal (e.g., @system.io)
	hash_tkn,	// hydro identity literal (e.g, #elementID)
	directive_tkn  // directive
};

//typedef class shared_ptr<class Token> lex_token;
typedef class Token *lex_token;

class Token
{
	friend class Lexer;

protected:
	std::string _src;
	std::string _value;
	int32_t _type;
	uint32_t _lineNum;
	int32_t _pos;
	lex_token _prevToken;
	lex_token _nextToken;

public:
	//Token(); // default
	Token(lex_source source, std::string value, int32_t type, uint32_t lineNum, int32_t pos);
	Token(std::string value, int32_t type, uint32_t lineNum = 0, int32_t pos = -1);
	//Token(const Token &token); // copy
	//Token(Token &&token); // move
	virtual ~Token();
	void insert(lex_token token);
	void remove();  // remove only
	void dispose(); // remove and delete
	std::string source() const;
	uint32_t line() const;
	int32_t position() const;
	std::string value() const;
	int32_t type() const;
	lex_token top()
	{
		lex_token token = _prevToken;

		while (token)
		{
			if (token->_prevToken)
				token = token->_prevToken;
			else
				break; // success!
		}

		// fail
		return token;
	}
	lex_token prev() const;
	lex_token next() const;
	lex_token begin();
	lex_token end();
	lex_token siblingAt(int32_t k);
	uint32_t getTokenIndex();
	virtual void dump();
	uint32_t length() const { return (uint32_t)_value.length(); }
};

} // namespace hydro

#endif /* __h3o_Token__ */
