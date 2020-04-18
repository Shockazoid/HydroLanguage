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

#include "HydroLexer.hpp"
#include "HydroCompiler.hpp"

namespace hydro
{

static std::regex h3olexer_package{"^@[a-zA-Z0-9_]+(\\.[a-zA-Z0-9_]+)*$"};
static std::regex h3olexer_hash{"^#[a-zA-Z_][a-zA-Z0-9_]*$"};

static std::regex h3olexer_operator{"^(\\+|\\+\\+|\\-|\\-\\-|\\*|\\*\\*|\\/|%|=|==|!=|===|~|!|\\.|&|&&|@>|@>>|<->|->|\\+=|-=|\\*=|\\*\\*=|\\/=|%=|&=|>|>=|<|<=|<=>|>>|<<|>>=|<<=|<<<|<<<=|>>>|>>>=|\\^|\\^=|\\||\\|\\||\\?|:|::|:::|::::|\\(|\\)|\\.|\\.\\.|\\.\\.\\.)$"};
static std::regex h3olexer_token{"^\\{|\\}|\\(|\\)|\\[|\\]|;|\\:|,$"};

HydroLexer::HydroLexer(HydroCompiler *compiler) : Lexer{compiler}
{
	registerTokenPattern(ws_tkn, lexer_whitespace);
	registerTokenPattern(id_tkn, lexer_identifier);
	registerTokenPattern(scomment_tkn, lexer_single_line_comment);

	registerTokenPattern(hash_tkn, h3olexer_hash); // identity reference
	registerTokenPattern(token_tkn, h3olexer_token);
	registerTokenPattern(op_tkn, h3olexer_operator);

	registerTokenName(hash_tkn, "identity reference");
	registerTokenName(package_tkn, "package reference");
}

HydroLexer::~HydroLexer() {}

bool HydroLexer::predictHex()
{
	specStart();

	if (specMatch('0') && (specMatch('x') || specMatch('X')))
	{
		// must be at least one hex digit
		if (specMatch(lexer_hexadecimal_digit))
		{
			while (specMatch(lexer_hexadecimal_digit))
				; // eat hex digits

			if (validateNum())
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroLexer::predictBin()
{
	specStart();

	if (specMatch('0') && (specMatch('b') || specMatch('B')))
	{
		while (specMatch(lexer_binary_digit))
			;

		// any trailing characters result in failure
		while (specMatch(lexer_alphanum_underscore))
			return specFail();

		// otherwise success!
		return specSuccess();
	}

	return specFail();
}

bool HydroLexer::predictOct()
{
	specStart();

	if (specMatch('0') && (specMatch(lexer_octal_digit)))
	{
		while (specMatch(lexer_octal_digit))
			;

		// any trailing characters result in failure
		while (specMatch(lexer_alphanum_underscore))
			return specFail();

		// otherwise success!
		return specSuccess();
	}

	return specFail();
}

bool HydroLexer::predictInt(bool bypassCheck)
{
	specStart();

	// must be at least one digit
	if (specMatch(lexer_digit))
	{
		while (specMatch(lexer_digit))
			; // eat all digits

		if (specMatch('i') || specMatch('I'))
		{
			// eat i
		}

		if (bypassCheck || validateNum())
			return specSuccess();
	}

	return specFail();
}

bool HydroLexer::predictShort()
{
	specStart();

	if (predictInt() && specPrev() != 'i' && specPrev() != 'I')
	{
		if (specMatch('s') || specMatch('S'))
		{
			if (validateNum())
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroLexer::predictLong()
{
	specStart();

	if (predictInt() && specPrev() != 'i' && specPrev() != 'I')
	{
		if (specMatch('l') || specMatch('L'))
			return specSuccess();
	}

	return specFail();
}

bool HydroLexer::predictUint(bool bypassCheck)
{
	specStart();

	if (predictInt() && specPrev() != 'i' && specPrev() != 'I')
	{
		if (specMatch('u') || specMatch('U'))
		{
			if (specMatch('i') || specMatch('I'))
			{
				// eat optional i
			}

			if (bypassCheck || validateNum())
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroLexer::predictUshort()
{
	specStart();

	if (predictUint() && specPrev() != 'i' && specPrev() != 'I')
	{
		if (specMatch('s') || specMatch('S'))
		{
			if (validateNum())
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroLexer::predictUlong()
{
	specStart();

	if (predictUint() && specPrev() != 'i' && specPrev() != 'I')
	{
		if (specMatch('l') || specMatch('L'))
		{
			if (validateNum())
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroLexer::predictExp()
{
	specStart();

	if (specMatch('e') || specMatch('E'))
	{
		if (!specMatch('+'))
			specMatch('-'); // both are optional

		if (specMatch(lexer_digit))
		{
			while (specMatch(lexer_digit))
				; // eat digits

			// do not check value for trailing alphanumeric characters and underscores (i.e., validateNum())
			return specSuccess();
		}
	}

	return specFail();
}

bool HydroLexer::predictFloat()
{
	specStart();

	if (
	    (predictDouble() && specPrev() != 'd' && specPrev() != 'D') ||
	    (predictInt() && specPrev() != 'i' && specPrev() != 'I'))
	{
		if (specMatch('f') || specMatch('F'))
		{
			if (validateNum())
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroLexer::predictDouble(bool bypassCheck)
{
	specStart();

	if (specMatch(lexer_digit))
	{
		while (specMatch(lexer_digit))
			; // eat digits

		// check for double to int syntax (no decimal)
		if (specMatch('d') || specMatch('D'))
		{
			if (bypassCheck || validateNum())
				return specSuccess();
		}

		// check for exponential or decimal
		if (predictExp())
		{
			if (specMatch('d') || specMatch('D'))
			{
				// eat optional d
			}

			if (bypassCheck || validateNum())
				return specSuccess();
		}
		else if (specMatch('.'))
		{
			// must have at least one digit after the decimal
			if (specMatch(lexer_digit))
			{
				while (specMatch(lexer_digit))
					; // eat digits

				if (predictExp())
				{
					// eat optional exponential
				}

				if (specMatch('d') || specMatch('D'))
				{
					// eat optional d
				}

				if (bypassCheck || validateNum())
					return specSuccess();
			}
		}
	}

	return specFail();
}

bool HydroLexer::validateNum()
{
	specStart();

	while (specMatch(lexer_alphanum_underscore))
		return specFail();

	return specSuccess();
}

lex_token HydroLexer::id()
{
	while (matchBufferWithNext(id_tkn))
		consume();

	return flush(buffer() == "true" || buffer() == "false" ? bool_tkn : id_tkn);
}

lex_token HydroLexer::hex()
{
	consume(); // eat 0
	consume(); // eat x

	while (matchBufferWithNext(lexer_hexadecimal))
		consume();

	if (matchBufferWithNext(lexer_alpha))
	{
		// not a valid hex
		advance();
		abrupt();
	}

	return flush(hex_tkn);
}

lex_token HydroLexer::bin()
{
	consume(); // eat 0
	consume(); // eat b

	while (matchBufferWithNext(lexer_binary))
		consume();

	return flush(bin_tkn);
}

lex_token HydroLexer::oct()
{
	consume(); // eat 0

	while (matchBufferWithNext(lexer_octal))
		consume();

	return flush(oct_tkn);
}

lex_token HydroLexer::u16()
{
	consume(); // eat first digit

	while (matchBufferWithNext(lexer_digit))
		consume();

	consume(); // eat u
	consume(); // eat s

	return flush(ushort_tkn);
}

lex_token HydroLexer::i16()
{
	consume(); // eat first digit

	while (matchBufferWithNext(lexer_digit))
		consume();

	consume(); // eat s

	return flush(short_tkn);
}

lex_token HydroLexer::u32()
{
	consume(); // eat first digit

	while (matchBufferWithNext(lexer_digit))
		consume();

	consume(); // eat u
	if (matchNext('i') || matchNext('i'))
		consume(); // eat optional i

	return flush(uint_tkn);
}

lex_token HydroLexer::i32()
{
	consume(); // eat first digit
	//std::cout << "token val_a: " << std::string{1, la(0)} << " [" << specPos() << " / " << source()->position() << "]" << std::endl;

	while (matchBufferWithNext(lexer_digit))
		consume(); // eat digits

	if (matchNext('i') || matchNext('I'))
		consume(); // eat optional i

	//std::cout << "token val_b: " << std::string{1, la(0)} << " [" << specPos() << " / " << source()->position() << "]" << std::endl;
	return flush(int_tkn);
}

lex_token HydroLexer::u64()
{
	consume(); // eat first digit

	while (matchBufferWithNext(lexer_digit))
		consume();

	consume(); // eat u
	consume(); // eat l

	return flush(ulong_tkn);
}

lex_token HydroLexer::i64()
{
	consume(); // eat first digit

	while (matchBufferWithNext(lexer_digit))
		consume();

	consume(); // eat l

	return flush(long_tkn);
}

lex_token HydroLexer::exp()
{
	consume(); // eat e

	if (matchNext('+') || matchNext('-'))
		consume(); // consume optional sign

	while (matchNext(lexer_digit))
		consume(); // consume exponential

	// float
	if (matchNext('f') || matchNext('F'))
	{
		consume();
		return flush(float_tkn);
	}
	else if (matchNext('d') || matchNext('D'))
		consume(); // eat optional d

	// double
	return flush(double_tkn);
}

lex_token HydroLexer::flt()
{
	consume(); // eat first digit

	while (matchNext(lexer_digit))
		consume(); // eat digit

	if (matchNext('e') || matchNext('E'))
		return exp();

	if (matchNext('.'))
	{
		consume(); // eat .
		while (matchNext(lexer_digit))
			consume(); // eat digits
	}

	if (matchNext('e') || matchNext('E'))
		return exp();

	consume(); // eat f

	return flush(float_tkn);
}

lex_token HydroLexer::dbl()
{
	consume(); // eat first digit

	while (matchNext(lexer_digit))
		consume(); // eat digit

	if (matchNext('e') || matchNext('E'))
		return exp();

	if (matchNext('.'))
	{
		consume(); // eat .
		while (matchNext(lexer_digit))
			consume(); // eat digits
	}

	if (matchNext('e') || matchNext('E'))
		return exp();

	if (matchNext('d') || matchNext('D'))
		consume(); // eat optional d

	return flush(double_tkn);
}

lex_token HydroLexer::chr()
{
	consume(); // eat '

	if (matchNext('\\'))
	{
		// escape sequence
		consume(); // eat reverse solidus

		switch (next())
		{
		case 'a':  // alert
		case 'b':  // backspace
		case 'e':  //
		case '?':  // question mark
		case '\'': // apostrophe
		case '"':  // solidus
		case '\\': // reverse solidus
		case 'n':  // linefeed
		case 't':  // horizontal tab
		case 'v':  // vertical tab
		case 'r':  // carriage return
		case 'f':  // formfeed
		{
			consume(); // eat escape sequence
			break;
		}
		case 'u':
		{
			// unicode (under 1000)
			// TODO do unicode
			break;
		}
		case 'U':
		{
			// unicode
			// TODO do unicode
			break;
		}
		default:
		{
			if (matchNext(lexer_digit))
			{
				// TODO octal
			}
			else if (nextExists())
				appendError(HVM_COMPILE_ERROR_UNKNOWN_ESCAPE_SEQUENCE, line(), position());
			else
				abrupt();
		}
		}
	}
	else
		consume(); // eat utf

	if (!matchNext('\''))
		abrupt();

	consume(); // eat '
	return flush(char_tkn);
}

lex_token HydroLexer::string()
{
	bool terminated = false;
	consume(); // eat "

	while (nextExists() && !terminated)
	{
		if (matchNext('\\'))
		{
			// escape sequence
			consume(); // eat reverse solidus

			switch (next())
			{
			case 'a':  // alert
			case 'b':  // backspace
			case 'e':  //
			case '?':  // question mark
			case '\'': // apostrophe
			case '"':  // solidus
			case '\\': // reverse solidus
			case 'n':  // linefeed
			case 't':  // horizontal tab
			case 'v':  // vertical tab
			case 'r':  // carriage return
			case 'f':  // formfeed
			{
				consume(); // eat escape sequence
				break;
			}
			case 'u':
			{
				// unicode (under 1000)
				// TODO do unicode
				break;
			}
			case 'U':
			{
				// unicode
				// TODO do unicode
				break;
			}
			default:
			{
				if (matchNext(lexer_digit))
				{
					// TODO octal
				}
				else if (nextExists())
					appendError(HVM_COMPILE_ERROR_UNKNOWN_ESCAPE_SEQUENCE, line(), position());
				else
					abrupt();
			}
			}
		}
		else if (matchNext('"'))
		{
			consume(); // eat "
			terminated = true;
		}
		else
			consume(); // eat any
	}

	if (!terminated)
		_compiler->appendError(HVM_COMPILE_ERROR_UNTERMINATED_STRING_LITERAL, source()->location(), line(), position());

	return flush(string_tkn);
}

lex_token HydroLexer::tpl()
{
	consume(); // eat @
	consume(); // eat "
	return flush(template_tkn);
}

lex_token HydroLexer::hash()
{
	consume(); // eat #

	while (matchBufferWithNext(hash_tkn))
		consume();

	return flush(hash_tkn);
}

lex_token HydroLexer::pkg()
{
	consume(); // eat $

	do
	{
		consume(); // eat

		if (matchCurrent('.'))
		{
			if (!matchNext(lexer_alphanum_underscore))
			{
				advance(); // to erroroneous token
				abrupt();
			}
		}
	} while (matchNext(lexer_alphanum_underscore) || matchNext('.'));

	return flush(package_tkn);
}

lex_token HydroLexer::dir()
{
    consume(); // eat @

    do
    {
        consume();
    }
    while (matchNext(lexer_alphanum_underscore));

    return flush(package_tkn);
}


lex_token HydroLexer::com1()
{
	// single line comment
	consume(); // eat /
	consume(); // eat /

	while (!matchNext('\n') && nextExists())
		consume();

	return flush(scomment_tkn);
}

lex_token HydroLexer::com2()
{
	// mult-line comment
	uint32_t depth = 1;
	consume(); // eat /
	consume(); // eat *

	while (source()->hasNext())
	{
		if (matchNext('*') && matchLa('/', 2))
		{
			consume();
			consume();
			depth--;
			if (depth == 0)
				break;
		}
		else if (matchNext('/') && matchLa('*', 2))
		{
			consume();
			consume();
			depth++;
		}
		else
			consume(); // eat any
	}

	return flush(mcomment_tkn);
}

lex_token HydroLexer::readNext()
{
	if (isEof(1))
	{
		consume();
		return flush(eof_tkn);
	}
	else if (matchNext('\n'))
	{
		consume();
		return flush(ln_tkn);
	}
	else if (matchNext(id_tkn))
		return id();
	else if (matchNext(lexer_whitespace))
	{
		do
			consume();
		while (matchNext(lexer_whitespace));

		return flush(ws_tkn);
	}
	else if (predictFloat())
		return flt();
	else if (predictDouble(false))
		return dbl();
	else if (predictHex())
		return hex();
	else if (predictBin())
		return bin();
	else if (predictOct())
		return oct();
	else if (predictUshort())
		return u16();
	else if (predictShort())
		return i16();
	else if (predictUint(false))
		return u32();
	else if (predictInt(false))
		return i32();
	else if (predictUlong())
		return u64();
	else if (predictLong())
		return i64();
	else if (matchNext(lexer_digit))
	{
		appendError(HVM_COMPILE_ERROR_INVALID_NUMBER_SYNTAX, line(), position());
	}
	else if (matchNext('\'')) // assume char
		return chr();
	else if (matchNext('"')) // assume string literal
		return string();
	else if (matchNext('@') && matchLa('"', 2)) // assume string template
		return tpl();
	else if (matchNext('$') && matchLa(lexer_alphanum_underscore, 2))
		return pkg(); // package reference
    else if (matchNext('@') && matchLa(lexer_alphanum_underscore, 2))
        return dir(); // directive
	else if (matchNext('#') && (matchLa(lexer_alphanum, 2) || matchLa('_', 2))) // assume hash
		return hash();
	else if (matchNext('/') && matchLa('/', 2))
		return com1(); // single line comment
	else if (matchNext('/') && matchLa('*', 2))
		return com2(); // mult-line comment
	else if (lex_token op = devour(op_tkn))
		return op;
	else if (lex_token token = devour(token_tkn))
		return token;

	consume();
	return flush(token_tkn);
}

} // namespace hydro
