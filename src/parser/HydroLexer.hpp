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

#ifndef __h3o_HydroLexer__
#define __h3o_HydroLexer__

#include "Lexer.hpp"

namespace hydro
{

class HydroLexer : public Lexer
{
private:
	// speculation
	bool predictHex();
	bool predictBin();
	bool predictOct();
	bool predictInt(bool bypassCheck = true);
	bool predictShort();
	bool predictLong();
	bool predictUint(bool bypassCheck = true);
	bool predictUshort();
	bool predictUlong();
	bool predictExp();
	bool predictFloat();
	bool predictDouble(bool bypassCheck = true);
	bool validateNum();

	// consumption
	lex_token id();     // identifier
	lex_token hex();    // hexadecimal literal
	lex_token bin();    // binary literal
	lex_token oct();    // octal literal
	lex_token u16();    // ushort literal
	lex_token i16();    // short literal
	lex_token u32();    // uint literal
	lex_token i32();    // int literal
	lex_token u64();    // ulong litearl
	lex_token i64();    // long literal
	lex_token exp();    // scientific notation (float/double)
	lex_token flt();    // float literal
	lex_token dbl();    // double literal
	lex_token chr();    // character literal
	lex_token string(); // string literal
	lex_token tpl();    // string template
	lex_token hash();   // identity reference
	lex_token pkg();    // package reference
    lex_token dir();    // directive
	lex_token com1();   // single line comment
	lex_token com2();   // multi-line comment

public:
	HydroLexer(class HydroCompiler *compiler);
	virtual ~HydroLexer();
	virtual lex_token readNext() override;
};

typedef HydroLexer *hydro_lexer;

} // namespace hydro

#endif /* __h3o_HydroLexer__ */
