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

#include <stack>

#include "HydroParser.hpp"
#include "HydroCompiler.hpp"

namespace hydro
{

HydroParser::HydroParser(HydroCompiler *compiler, Program *program, Scope *globalScope) : Parser{new HydroLexer{compiler}}, _program{program}, _globalScope{globalScope}
{
	ignore(ws_tkn);	 // ignore whitespace
	ignore(scomment_tkn); // ignore single line comments
	ignore(mcomment_tkn); // ignore multi-line comments
}

HydroParser::~HydroParser() {}

bool HydroParser::specEos()
{
	specStart();

	if (specMatch(";") || specMatch("\n"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specAllowLine()
{
	specSkip(ln_tkn);
	return specExists();
}

bool HydroParser::specID()
{
	return specMatch(id_tkn);
}

bool HydroParser::specBool()
{
	return specMatch(bool_tkn);
}

bool HydroParser::specChar()
{
	return specMatch(char_tkn);
}

bool HydroParser::specString()
{
	return specMatch(string_tkn);
}

bool HydroParser::specStringTemplate()
{
	specStart();

	if (specMatch(template_tkn))
		return specSuccess();

	return specFail();
}

bool HydroParser::specBinary()
{
	return specMatch(bin_tkn);
}

bool HydroParser::specHexadecimal()
{
	return specMatch(hex_tkn);
}

bool HydroParser::specOctal()
{
	return specMatch(oct_tkn);
}

bool HydroParser::specUshort()
{
	return specMatch(ushort_tkn);
}

bool HydroParser::specShort()
{
	return specMatch(short_tkn);
}

bool HydroParser::specUint()
{
	return specMatch(uint_tkn);
}

bool HydroParser::specInt()
{
	return specMatch(int_tkn);
}

bool HydroParser::specUlong()
{
	return specMatch(ulong_tkn);
}

bool HydroParser::specLong()
{
	return specMatch(long_tkn);
}

bool HydroParser::specFloat()
{
	return specMatch(float_tkn);
}

bool HydroParser::specDouble()
{
	return specMatch(double_tkn);
}

bool HydroParser::specNumber()
{
	return specBinary() ||
		 specHexadecimal() ||
		 specOctal() ||
		 specUshort() ||
		 specShort() ||
		 specUint() ||
		 specInt() ||
		 specUlong() ||
		 specLong() ||
		 specFloat() ||
		 specDouble();
}

bool HydroParser::specNaN()
{
	return specMatch("NaN");
}

bool HydroParser::specInfinity()
{
	return specMatch("Infinity");
}

bool HydroParser::specNull()
{
	return specMatch("null");
}

bool HydroParser::specUndefined()
{
	return specMatch("undefined");
}

bool HydroParser::specIdentityRef()
{
	return specMatch(hash_tkn); // #identity
}

bool HydroParser::specPackageRef()
{
	return specMatch(package_tkn); // @com.example
}

bool HydroParser::specDirective()
{
	return specMatch(directive_tkn); // $$directive ... $/directive
}

bool HydroParser::assumeObject()
{
	specStart();

	if (specMatch("{"))
	{
		specSkip(ln_tkn); // allow newline

		// if assumed JSON literal
		if (specMatch("}") || specMatch(string_tkn))
			return specFail();

		// object initializer has priority over JSON literal
		return specSuccess();
	}

	return specFail();
}

bool HydroParser::specObject()
{
	specStart();

	// full speculation
	if (specMatch("{"))
	{
		specSkip(ln_tkn); // allow newline

		if (specMatch(":"))
		{

			specSkip(ln_tkn); // allow newline

			if (specMatch("}"))
				return specSuccess(); // empty object
			else
			{
				while (true)
				{
					specSkip(ln_tkn); // allow newline

					if (specID())
					{
						specSkip(ln_tkn); // allow newline

						if (specMatch(":"))
						{
							specSkip(ln_tkn); // allow newline

							if (specExpr(binaryPrec(",") + 1))
							{
								specSkip(ln_tkn); // allow newline

								if (specMatch(","))
									continue;
								else if (specMatch("}"))
									return specSuccess();
							}
						}
					}

					break; // fail
				}
			}
		}
	}

	return specFail();
}

bool HydroParser::assumeJson()
{
	specStart();

	if (specMatch("{") && (specMatch(string_tkn) || specMatch("}")))
		return specSuccess();

	return specFail();
}

bool HydroParser::specJson()
{
	specStart();

	// TODO full JSON speculation
	if (specMatch("{") && specString() && specMatch(":"))
		return specSuccess();

	return specFail();
}

bool HydroParser::assumeArray()
{
	specStart();

	if (specMatch("["))
		return specSuccess();

	return specFail();
}

bool HydroParser::specArray()
{
	specStart();

	if (specMatch("["))
	{
		if (specMatch("]", ln_tkn))
			return specSuccess();
		else
		{

			while (true)
			{
				specSkip(ln_tkn); // allow newline

				if (specExpr(binaryPrec(",") + 1))
				{
					specSkip(ln_tkn); // allow newline

					if (specMatch(","))
						continue;
					else if (specMatch("]"))
						return specSuccess();
				}

				break; // fail
			}
		}
	}

	return specFail();
}

bool HydroParser::assumeDict()
{
	specStart();

	if (specMatch("["))
	{
		// lazy speculation
		specSkip(ln_tkn); // allow newline

		if (specMatch(":"))
		{
			specSkip(ln_tkn); // allow newline

			return specSuccess(); // empty dictionary
		}
		else if (specExpr())
		{
			specSkip(ln_tkn); // allow newline

			if (specMatch(":"))
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroParser::specDict()
{
	specStart();

	if (specMatch("["))
	{
		specSkip(ln_tkn); // allow newline

		if (specMatch(":"))
		{
			specSkip(ln_tkn); // allow newline

			if (specMatch("]"))
				return specSuccess(); // empty dictionary
		}
		else
		{
			// full dictionary speculation
			while (true)
			{
				specSkip(ln_tkn); // allow newline

				if (specExpr(binaryPrec(",") + 1))
				{
					specSkip(ln_tkn); // allow newline

					if (specMatch(":"))
					{
						specSkip(ln_tkn); // allow newline

						if (specExpr(binaryPrec(",") + 1))
						{
							specSkip(ln_tkn); // allow newline

							if (specMatch(","))
								continue;
							else if (specMatch("]"))
								return specSuccess();
						}
					}
				}

				break; // fail
			}
		}
	}

	return specFail();
}

bool HydroParser::assumeRegex()
{
	return specMatch("/");
}

bool HydroParser::specRegex()
{
	return specMatch("/");
}

bool HydroParser::assumeXml()
{
	return specMatch("<");
}

bool HydroParser::specXml()
{
	// TODO full xml speculation
	return specMatch("<");
}

bool HydroParser::specNestedExpr()
{
	specStart();

	if (specMatch("("))
	{
		specSkip(ln_tkn); // allow newline

		if (specExpr())
		{
			specSkip(ln_tkn); // allow newline

			if (specMatch(")"))
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroParser::assumeTuple()
{
	specStart();

	if (specMatch("("))
	{
		specSkip(ln_tkn); // allow newline

		if (specMatch(")"))
			return specSuccess(); // empty tuple

		if (specExpr(binaryPrec(",") + 1))
		{
			specSkip(ln_tkn); // allow newline

			if (specMatch(","))
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroParser::specTuple()
{
	specStart();
	long n = 0;

	if (specMatch("("))
	{
		if (specMatch(")"))
			return specSuccess(); // empty tuple

		while (true)
		{
			specSkip(ln_tkn); // allow newline

			if (specMatch(")"))
				return specSuccess();

			if (specExpr())
			{
				specSkip(ln_tkn); // allow newline

				n++;
				if (specMatch(")"))
					return n > 1 ? specSuccess() : specFail();
				else if (specMatch(","))
					continue;
			}

			// fail
			break;
		}
	}

	return specFail();
}

bool HydroParser::assumeNamedTuple()
{
	specStart();

	if (specMatch("("))
	{
		specSkip(ln_tkn); // allow newline

		if (specMatch(")"))
			return specSuccess(); // empty tuple
		else if (specID())
		{
			specSkip(ln_tkn); // allow newline

			if (specMatch(":"))
				return specSuccess();
		}
	}

	return specFail();
}

bool HydroParser::specNamedTuple()
{
	specStart();

	if (specMatch("("))
	{
		specSkip(ln_tkn); // allow newline

		if (specMatch(")"))
			return specSuccess(); // empty tuple

		while (true)
		{
			specSkip(ln_tkn); // allow newline

			if (specID())
			{
				specSkip(ln_tkn); // allow newline

				if (specMatch(":") && specExpr())
				{
					specSkip(ln_tkn); // allow newline

					if (specMatch(")"))
						return specSuccess();
					else if (specMatch(","))
						continue;
				}
			}

			// fail
			break;
		}
	}

	return specFail();
}

bool HydroParser::assumeNamedArgs()
{
    specStart();
    
    if(specMatch("(") && specMatch(id_tkn) && specMatch(":"))
        return specSuccess();
    
    return specFail();
}

bool HydroParser::specAfunc()
{
	specStart();

	if (specMatch("func") && specMatch("("))
		return specSuccess();

	return specFail();
}

bool HydroParser::specExpr(int minPrec)
{
	specStart();

	if (specOpnd())
	{
		while (specExists())
		{

			if (specMatch(")") || specMatch("]") || specMatch(":") || specMatch("{") || specMatch("}")) // primaries, delimiters, and general tokens
			{
				specDown();		    // back track
				return specSuccess(); // end
			}
			else if (specMatch("[")) // index access
			{
				if (specExpr() && specMatch("]"))
					continue; // valid
				else
					return specFail();
			}
			else if (specMatch("(")) // invocation
			{
				if (specMatch(")")) // zero arguments
					continue;     // valid
				else			  // at least one argument
				{
					if (specExpr() && specMatch(")"))
						continue;
					else
						return specFail();
				}
			}
			else if (specMatch("?"))
			{
				if (specExpr() && specMatch(":") && specExpr())
					continue;
				else
					return specFail();
			}
			else if (specMatch(")") || specMatch("]") || (specMatch(":") || specMatch("{") || specMatch("}")))
				return specFail(); // invalid
			else if (
			    (specMatch(op_tkn) || specMatch("as") || specMatch("is") || specMatch("instanceof") || specMatch(",")) &&
			    (binaryPrec(laVal(specPos() - 1)) >= minPrec))
			{
				if (specOpnd())
					continue; // valid
				else
					return specFail();
			}

			return specSuccess(); // end
		}
	}

	return specFail();
}

bool HydroParser::specExpr()
{
	return specExpr(-1);
}

bool HydroParser::assumeOpnd()
{
	specStart();

	if (
	    specNumber() ||	// float, integer, hexadecimal, octal, binary, or double
	    specNaN() ||		 // NaN
	    specInfinity() ||    // Infinity
	    specUndefined() ||   // undefined
	    specNull() ||		 // null
	    specBool() ||		 // true or false
	    specString() ||	// "string"
        specStringTemplate() || // string template
	    specIdentityRef() || // #ID
	    specMatch("{") ||    // object or JSON initializer
	    specMatch("[") ||    // array or dictionary initializer
	    specMatch("(") ||    // nested expression or tuple
	    specMatch("<") ||    // XML literal
	    specMatch("/") ||    // regular expression literal
	    assumeUnaryOp() ||
	    specID() // Indentifier (name)
	)
		return specSuccess();

	return specFail();
}

bool HydroParser::specOpnd()
{
	specStart();

	if (
	    specNumber() ||
	    specNaN() ||
	    specInfinity() ||
	    specUndefined() ||
	    specNull() ||
	    specBool() ||
	    specString() ||
	    specStringTemplate() ||
	    specIdentityRef() ||
	    specJson() ||
	    specObject() ||
	    specDict() ||
	    specArray() ||
	    specXml() ||
	    specRegex() ||
	    specTuple() ||
	    specNamedTuple() ||
	    specID() ||
	    specUnaryOp() ||
	    specMatch("#") ||
	    specMatch("$"))
		return specSuccess();

	return specFail();
}

bool HydroParser::assumeUnaryOp()
{
	specStart();

	if (specMatch(op_tkn) ||
	    ((specMatch("new") || specMatch("throw") || specMatch("delete") || specMatch("typeof") || specMatch("classof")) && assumeOpnd()))
		return specSuccess();

	return specFail();
}

bool HydroParser::specUnaryOp()
{
	specStart();

	if (((specMatch(op_tkn) || specMatch("throw") || specMatch("delete") || specMatch("typeof") || specMatch("classof") || specMatch("new")) && specExpr()) ||
	    (specMatch("(") && specAllowLine() && specExpr() && specAllowLine() && specMatch(")")))
		return specSuccess(); // assume unary

	return specFail();
}

bool HydroParser::specImport()
{
	specStart();

	while (specMatch("import") && (specMatch(package_tkn) || specMatch(id_tkn)))
		return specSuccess();

	return specFail();
}

bool HydroParser::isModifier(std::string value)
{
	return value == "public" ||     // global access
		 value == "internal" ||     // package access
		 value == "protected" ||    // class and sub-class access
		 value == "restricted" ||   // class and instance access
		 value == "private" ||      // class access
		 value == "native" ||       // native entity
		 value == "abstract" ||     // abstract entity
		 value == "override" ||     // method override
		 value == "dynamic" ||      // dynamic class
		 value == "static" ||       // static entity
		 value == "final" ||        // prevents member override
         value == "readonly" ||     //
         value == "writeonly" ||    //
         value == "synchronized"    //
	    ;
}

bool HydroParser::specModifiers(bool modRequired)
{
	specStart();
	bool success = !modRequired;

	while (
	    specMatch("public") ||
	    specMatch("internal") ||
	    specMatch("protected") ||
	    specMatch("restricted") ||
	    specMatch("private") ||
	    specMatch("native") ||
	    specMatch("abstract") ||
	    specMatch("override") ||
	    specMatch("static") ||
	    specMatch("dynamic") ||
	    specMatch("final") ||
        specMatch("readonly") ||
        specMatch("writeonly") ||
	    specMatch("synchronized"))
		success = true;

	return success ? specSuccess() : specFail();
}

bool HydroParser::specPackage()
{
    specStart();
    
    if(specMatch("package") && specMatch(id_tkn))
        return specSuccess();
    
    return specFail();
}

bool HydroParser::specLibrary()
{
    specStart();
    
    if(specMatch("library") && specMatch(id_tkn))
        return specSuccess();
    
    return specFail();
}

bool HydroParser::specVar()
{
	specStart();

	if (specModifiers() && (specMatch("var") || specMatch("const") || specMatch("let")) && specID())
		return specSuccess();

	return specFail();
}

bool HydroParser::specBlock()
{
	specStart();

	while (specMatch(ln_tkn))
		;

	if (specMatch("{"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specNilBlock()
{
	specStart();

	while (specMatch(ln_tkn))
		;

	if (!specMatch("{"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specIf()
{
	return specMatch("if");
}

bool HydroParser::specElseIf()
{
	specStart();

	if (specMatch("else") && specMatch("if"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specElse()
{
	specStart();

	if (specMatch("else"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specTry()
{
	specStart();

	if (specMatch("try") && specMatch("{", ln_tkn))
		return specSuccess();

	return specFail();
}

bool HydroParser::specCatch()
{
	specStart();

	if (specMatch("catch") && (specMatch(id_tkn) || specMatch(package_tkn)))
		return specSuccess();

	return specFail();
}

bool HydroParser::assumeThrow()
{
    specStart();
    
    if (specMatch("throw") && assumeOpnd())
        return specSuccess();
    
    return specFail();
}

bool HydroParser::specFor()
{
	specStart();

	if (specMatch("for"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specDoWhile()
{
	specStart();

	if (specMatch("do"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specWhile()
{
	specStart();

	if (specMatch("while"))
		return specSuccess();

	return specFail();
}

bool HydroParser::assumeFunc()
{
	specStart();

	if (specModifiers() && specMatch("func") && specID())
		return specSuccess();

	return specFail();
}

bool HydroParser::assumeAnonymousFunc()
{
    specStart();
    
    if(specModifiers() && specMatch("func") && specMatch("("))
        return specSuccess();
    
    return specFail();
}

bool HydroParser::assumeAction()
{
    specStart();
    
    if(specModifiers() && specMatch("act") && specID())
        return specSuccess();
    
    return specFail();
}

bool HydroParser::assumeAnonymousAction()
{
    specStart();
    
    if(specModifiers() && specMatch("act") && specMatch("["))
        return specSuccess();
    
    return specFail();
}

bool HydroParser::assumeReturn()
{
	specStart();

	if (specMatch("return"))
		return specSuccess();

	return specFail();
}

bool HydroParser::assumeEtype()
{
    specStart();
    
    if(specModifiers() && specMatch("etype") && specID())
        return specSuccess();
    
    return specFail();
}

bool HydroParser::assumeEvent()
{
    specStart();
    
    if(specModifiers() && specMatch("event") && specID())
        return specSuccess();
    
    return specFail();
}

bool HydroParser::assumeParam()
{
    specStart();
    
    if(specModifiers() && specMatch("param") && specID())
        return specSuccess();
    
    return specFail();
}

bool HydroParser::assumeClass()
{
	specStart();

	if (specModifiers() && specMatch("class") && specID())
		return specSuccess();

	return specFail();
}

bool HydroParser::specProperty()
{
	specStart();

	if (specModifiers() && specMatch("prop") && specID())
		return specSuccess();

	return specFail();
}

bool HydroParser::specGetter()
{
	specStart();

	if (specModifiers() && specMatch("[") && specMatch("get") && specMatch("]"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specSetter()
{
	specStart();

	if (specModifiers() && specMatch("[") && specMatch("set") && specMatch("]"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specConstructor()
{
	specStart();

	if (specModifiers(true) && specMatch("constructor"))
		return specSuccess();

	return specFail();
}

bool HydroParser::specMethod()
{
	specStart();

	if (specModifiers() && specMatch("meth") && specID())
		return specSuccess();

	return specFail();
}

bool HydroParser::specEvent()
{
    specStart();
    
    if(specModifiers() && specMatch("event") && specID())
        return specSuccess();
    
    return specFail();
}

bool HydroParser::specEtype()
{
    specStart();
    
    if(specMatch("etype"))
        return specSuccess();
    
    return specFail();
}

bool HydroParser::specAttribute()
{
    specStart();
    
    // TODO speculate attribute
    
    return specFail();
}

int HydroParser::binaryPrec(lex_token op)
{
	return binaryPrec(op->value());
}

int HydroParser::binaryPrec(std::string op)
{
	if (op == "(" || op == ")" || op == "[" || op == "]" || op == "." || op == "::")
		return 17; // primary
	else if (op == "->")
		return 16; // method chain
	else if (op == "**")
		return 15; // exponential
	else if (op == "*" || op == "/" || op == "%")
		return 14; // multiplicative
	else if (op == "+" || op == "-")
		return 13; // additive
	else if (op == "<<" || op == ">>" || op == ">>>")
		return 12; // bitwise shift
	else if (op == "<" || op == ">" || op == "<=" ||
		   op == ">=" || op == "is" || op == "as" ||
		   op == "in" || op == "instanceof")
		return 11; // relational
	else if (op == "==" || op == "!=" ||
		   op == "===" || op == "!==")
		return 10; // equality
	else if (op == "&")
		return 9; // bitwise AND
	else if (op == "^")
		return 8; // bitwise XOR
	else if (op == "|")
		return 7; // bitwise OR
	else if (op == "&&")
		return 6; // logical AND
	else if (op == "||")
		return 5; // logical OR
	else if (op == "?")
		return 4; // conditional
	else if (op == "=" || op == "+=" || op == "-=" ||
		   op == "**=" || op == "*=" || op == "/=" ||
		   op == "%=" || op == ">>=" || op == ">>>=" ||
		   op == "<<=" || op == "^=" || op == "&=" ||
		   op == "|=" || op == "<=>")
		return 3; // assignment
	else if (op == ":")
		return 2;
	else if (op == ",")
		return 1;

	return 0;
}

int HydroParser::binaryAssoc(lex_token op)
{
	return binaryAssoc(op->value());
}

int HydroParser::binaryAssoc(std::string op)
{
	if (
	    op == "=" ||
	    op == "**=" ||
	    op == "*=" ||
	    op == "/=" ||
	    op == "%=" ||
	    op == "+=" ||
	    op == "-=" ||
	    op == "&=" ||
	    op == "|=" ||
	    op == "^=" ||
	    op == ">>=" ||
	    op == ">>>=" ||
	    op == "<<=")
		return op_right_assoc; // right associative

	return op_left_assoc; // left associative
}

bool HydroParser::isBinaryOp(lex_token op)
{
	return isBinaryOp(op->value());
}

bool HydroParser::isBinaryOp(std::string op)
{
	return binaryPrec(op) > 0;
}

int HydroParser::unaryPrec(lex_token op)
{
	return unaryPrec(op->value());
}

int HydroParser::unaryPrec(std::string op)
{
	if (op == "(" || op == ")" || op == "[" || op == "]" || op == "new" || op == "|")
		return 16; // Primary
	else if (op == "++" || op == "--")
		return 15;
	else if (op == "!" ||
		   op == "+" ||
		   op == "-" ||
		   op == "~" ||
		   op == "@>" ||
		   op == "@>>" ||
		   op == "delete" ||
		   op == "throw")
		return 14;
	else if (op == "typeof" || op == "classof")
		return 13;

	return 0;
}

int HydroParser::postfixPrec(lex_token op)
{
	return postfixPrec(op->value());
}

int HydroParser::postfixPrec(std::string op)
{
	if (op == "++" || op == "--")
		return 16;

	return 0;
}

bool HydroParser::isPostfix(lex_token op)
{
	return isPostfix(op->value());
}

bool HydroParser::isPostfix(std::string op)
{
	return op == "++" || op == "--";
}

ast_expr HydroParser::computeExpr()
{
	return computeExpr(-1);
}

ast_expr HydroParser::computeExpr(int minPrec)
{
	std::stack<lex_token> operators;
	std::stack<ast_expr> operands;
	ast_expr expr;
	ast_expr lhs;
	ast_expr rhs;
	lex_token a;
	lex_token b;

	skip(ln_tkn); // skip trailing linefeeds

	operators.push(new Token{"[sentinel]", nil_tkn}); // push sentinel

	operands.push(parseOpnd()); // shift first operand (as expected)

	// Begin shift loop
	while (
	    nextTokenExists() &&
	    ((matchNext(op_tkn) || matchNext("as") || matchNext("is") || matchNext("instanceof") || matchNext("[") || matchNext("(")) &&
	     (binaryPrec(laVal(1)) >= minPrec)) &&
	    (!matchNext(")") && !matchNext("]") && !matchNext(":") && !matchNext("{") && !matchNext("}")) // Don't expect these to be binary operators
	)
	{
		advance();		  // To assumed binary or post-fix operator
		a = currentToken(); // Target operator

		if (!isBinaryOp(a) && !isPostfix(a))
		{
			appendError(HVM_COMPILE_ERROR_INVALID_BINARY, a);
			abort();
		}

		// Begin reduction loop
		while (true)
		{
			b = operators.top(); // Visit top of stack

			if (isPostfix(a) && postfixPrec(a) > binaryPrec(b))
			{
				// Operator b is not a primary
				if (match("++", a))
				{
					// post-fix increment
					lhs = operands.top();
					operands.pop();
					operands.push(new UnaryOp(a, lhs, true));
					break;
				}
				else if (match("--", a))
				{
					// post-fix decrement
					lhs = operands.top();
					operands.pop();
					operands.push(new UnaryOp(a, lhs, true));
					break;
				}
			}
			else if (binaryPrec(a) > binaryPrec(b) || (binaryPrec(a) == binaryPrec(b) && binaryAssoc(a) == op_right_assoc))
			{ // If a > b
                if (match("(", a))
				{
					// invocation
					lhs = operands.top();
					operands.pop();
					ast_cargs args = parseCallArgs();
					ast_call call = new Call(lhs, a, args);
					operands.push(call);
					break;
				}
				else if (match("[", a))
				{
					// subscript
					lhs = operands.top();
					operands.pop();
					advanceSkip(ln_tkn); // to expected expr (allow linefeed)
					rhs = computeExpr();
					advanceSkip(ln_tkn); // to expected ] (allow linefeed)
					validate("]");
					operands.push(new BinaryOp(lhs, a, rhs));
					break;
				}
				else if (match("?", a))
				{
					// conditional expression (expr ? block : block)
					ast_expr cond = operands.top();
					operands.pop();
					advanceSkip(ln_tkn); // to expected expr (allow linefeed)
					lhs = computeExpr(binaryPrec(a));
					advanceSkip(ln_tkn); // To expected : (allow linefeed)
					validate(":");
					b = currentToken();
					advanceSkip(ln_tkn); // to expected expr (allow linefeed)
					rhs = computeExpr(binaryPrec(a));
					operands.push(new TernaryOp(cond, a, lhs, b, rhs));
					break;
				}
				else
				{
					// push a onto operator stack and shift next operand
					operators.push(a);
					advanceSkip(ln_tkn); // to expected operand (allow linefeed)
					operands.push(parseOpnd());
					break;
				}
			}
			else
			{
				// reduce b
				rhs = operands.top();
				operands.pop(); // pop rhs
				lhs = operands.top();
				operands.pop(); // pop lhs
				operands.push(parseBinaryOp(lhs, b, rhs));
				operators.pop(); // pop reduced operator b
				continue;	  // continue reduction
			}
		}
	} // end shift loop

	// begin pop loop
	while (operators.size() > 1) // while sentinel is not the only token
	{
		a = operators.top();
		operators.pop();     // pop operator a
		b = operators.top(); // operator b

		if ((binaryPrec(a) > binaryPrec(b)) || (binaryPrec(a) == binaryPrec(b) && binaryAssoc(a) == op_right_assoc))
		{
			// reduce a
			rhs = operands.top();
			operands.pop(); // pop rhs
			lhs = operands.top();
			operands.pop(); // pop lhs
			//operands.push(new szd::BinaryOp(lhs, a, rhs));
			operands.push(parseBinaryOp(lhs, a, rhs));
		}
		else
		{
			// reduce b
			operators.pop(); // pop operator b
			rhs = operands.top();
			operands.pop(); // pop rhs
			lhs = operands.top();
			operands.pop(); // pop lhs
			operands.push(parseBinaryOp(lhs, b, rhs));
			operators.push(a); // restore operator a as the top of the stack
		}
	} // end pop loop

	operators.top(); // pop sentinel
	expr = operands.top();
	operands.pop(); // pop result

	return expr;
}

ast_expr HydroParser::emptyExpr()
{
	return new EmptyExpr{};
}

ast_expr HydroParser::parseOpnd()
{
	if ( // literals and references
	    specNumber() ||
	    specBool() ||
	    specChar() ||
	    specString() ||
	    specIdentityRef() ||
	    specPackageRef())
		return new Literal{currentToken()};
	else if (specStringTemplate())
		return parseStringTemplate();
	else if (assumeJson())
		return parseJson();
	else if (assumeObject())
		return parseObject();
	else if (assumeDict())
		return parseDict();
	else if (assumeArray())
		return parseArray();
	else if (assumeNamedTuple())
		return parseNamedTuple();
	else if (assumeTuple())
		return parseTuple();
	else if (match("(")) // nested expression
		return parseNested();
	//else if(assumeRegex())
	//    return parseRegex();
	//else if(assumeXml())
	//    return parseXml();
    //else if (assumeAnonymousFunc())
    //    return parseFuncExpr();
    //else if (assumeAnonymousAction())
    //    return parseActionExpr();
	else if (match(op_tkn) || match("new") || match("delete") || match("typeof") || match("classof"))
		return parseUnaryOp();
	else if (specID())
		return parseID();

	abrupt(); // throws error
	return nullptr;
}

ast_literal HydroParser::buildString(std::vector<lex_token> &stringTokens)
{
	lex_token tkn;

	if (!stringTokens.empty())
	{
		std::string str = "";
		str += '"';

		// merge tokens
		for (lex_token strTkn : stringTokens)
			str += strTkn->value();

		str += '"';
		tkn = new Token{str, string_tkn};
	}
	else
	{
		tkn = new Token{"\"\"", string_tkn};
	}

	stringTokens.clear();
	return new Literal{tkn};
}

Expr *HydroParser::parseStringTemplate()
{
	validate(template_tkn);
	lex_token tkn = currentToken();
	StringTemplate *tpl = new StringTemplate{tkn};
	Expr *expr;
	bool terminated = false;
	std::vector<lex_token> stringTokens;

	while (lexer()->hasNext())
	{
		advanceChar(); // to next char

		if (match("\\"))
		{
			// eat reverse solidus
			stringTokens.push_back(currentToken());
			advanceChar();

			if (!tokenExists())
			{
				// incomplete escape sequence
				appendError(HVM_COMPILE_ERROR_INCOMPLETE_ESCAPE_SEQUENCE, currentToken());
				abort();
			}

			char ch = tokenVal()[0];

			switch (ch)
			{
			case '{':
			case '}':
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
				// consume
				stringTokens.push_back(currentToken());
				break;
			}
			case 'u':
			{
				// unicode (under 1000)
				// TODO do unicode
				// consume u
				stringTokens.push_back(currentToken());
				appendError("Unicode is not current supported.", currentToken());
				abort();
				break;
			}
			case 'U':
			{
				// unicode
				// TODO do unicode
				// consume U
				stringTokens.push_back(currentToken());
				appendError("Unicode is not current supported.", currentToken());
				abort();
				break;
			}
			default:
			{
				// TODO parse octals here
				appendError(HVM_COMPILE_ERROR_UNKNOWN_ESCAPE_SEQUENCE, currentToken());
				abort();
			}
			}
		}
		else if (match("{"))
		{
			// build string
			tpl->append(buildString(stringTokens));
			advance(); // to expected expression
			expr = computeExpr();
			advance(); // to expected }
			validate("}");
			tpl->append(expr); // append expression
		}
		else if (match("\""))
		{
			// build string
			tpl->append(buildString(stringTokens));
			terminated = true;
			break;
		}
		else
		{
			// consume character
			stringTokens.push_back(currentToken());
			continue;
		}
	}

	if (!terminated)
        appendError(HVM_COMPILE_ERROR_UNTERMINATED_STRING_TEMPLATE, tpl->token());
    
	return tpl;
}

SimpleName *HydroParser::parseSimpleName()
{
	validate(id_tkn);
	return new SimpleName{currentToken()};
}

Name *HydroParser::parseName()
{
    validate(id_tkn);
    Name *result = nullptr;
    
    if(matchNext(".", ln_tkn))
    {
        Name *lhs;
        SimpleName *rhs;
        
        // qualified name
        result = parseSimpleName(); // will be lhs
        
        // qualified
        // com.example.MyClass
        while(matchNext(".", ln_tkn))
        {
            advanceSkip(ln_tkn); // to dot
            validate(".");
            lhs = result;
            advance(); // to expected name
            rhs = parseSimpleName();
            result = new QualifiedName{lhs, rhs};
        }
    }
    else
    {
        // simple name
        result = parseSimpleName();
    }
    
    return result;
}

ast_name HydroParser::parseID()
{
	validate(id_tkn);
	return new SimpleName{currentToken(), true}; // identifier
}

ast_package HydroParser::parsePackageSpec()
{
	validate(package_tkn);
	return new PackageSpec{currentToken()};
}

void HydroParser::parseNext()
{
	skip(ln_tkn);

	if (!match(eof_tkn) && tokenExists())
	{
		ast_stmt stmt = parseStmt(true);
		_program->append(stmt);
	}
}

void HydroParser::onParseInit()
{
    _currentScope = _globalScope; // push global scope
    // TODO re-implement file scope when ast owner is created (i.e., FileAst)
}

ast_unary HydroParser::parseUnaryOp()
{
	// Note: All of these are prefix unary operators
	lex_token tkn = currentToken();
	advance(); // to expected expr

	if (unaryPrec(tkn) > 0) // precedence will return a value if token is unary
	{
		return new UnaryOp(tkn, computeExpr(unaryPrec(tkn) + 1), false);
	}

	_compiler->appendError(HVM_COMPILE_ERROR_INVALID_UNARY, tkn);
	abort();
	return nullptr;
}

ast_binary HydroParser::parseBinaryOp(ast_expr lhs, lex_token token, ast_expr rhs)
{
	if (isBinaryOp(token))
		return new BinaryOp{lhs, token, rhs};

	appendError(HVM_COMPILE_ERROR_INVALID_BINARY, token);
	abort();
	return nullptr;
}

ast_cargs HydroParser::parseCallArgs()
{
    if(assumeNamedArgs())
        return parseNamedArgs();
    
	validate("(");
	ast_cargs args = new CallArgs{};
    
	if (!matchNext(")", ln_tkn))
	{
		while (nextTokenExists())
		{
			advanceSkip(ln_tkn); // to expected expression
			args->append(computeExpr());

			if (matchNext(",", ln_tkn))
				advanceSkip(ln_tkn); // to , (allow linefeed)
			else
				break;
		}
	}
    
    advanceSkip(ln_tkn);
    validate(")");
	return args;
}

NamedCallArgs *HydroParser::parseNamedArgs()
{
    validate("(");
    NamedCallArgs *args = new NamedCallArgs{};
    
    while(nextTokenExists())
    {
        advanceSkip(ln_tkn); // to expected name
        SimpleName *name = parseSimpleName();
        advanceSkip(ln_tkn); // to :
        validate(":");
        advanceSkip(ln_tkn); // to expected expr
        Expr *expr = computeExpr();
        args->append(new NamedArg{name, expr});
        
        if (matchNext(",", ln_tkn))
            advanceSkip(ln_tkn); // to , (allow linefeed)
        else
            break;
    }

    advanceSkip(ln_tkn);
    validate(")");
    return args;
}

ast_nested HydroParser::parseNested()
{
	// todo tuple
	// nested expression
	validate("(");
	lex_token tkn = currentToken();
	advance(); // to expected expression
	ast_expr expr = computeExpr();
	advance(); // to expected )
	validate(")");
	return new Nested{tkn, expr};
}

ast_tuple HydroParser::parseTuple()
{
	validate("(");
	ast_tuple tuple = new TupleInit{currentToken()};

	if (matchNext(")"))
	{
		advance();    // to )
		return tuple; // empty tuple
	}

	while (nextTokenExists())
	{
		advance(); // to expected expression
		ast_expr expr = computeExpr();
		tuple->append(expr);

		if (matchNext(","))
		{
			advance(); // to ,
			continue;
		}

		advance(); // to expected )
		validate(")");
	}

	return tuple;
}

ast_ntuple HydroParser::parseNamedTuple()
{
	validate("(");
	ast_ntuple tuple = new NamedTupleInit{currentToken()};

	if (matchNext(")"))
	{
		advance();    // to )
		return tuple; // empty tuple
	}

	while (nextTokenExists())
	{
		advance(); // to expected name
		ast_name name = parseSimpleName();
		advance(); // to expected :
		validate(":");
		lex_token tkn = currentToken();
		advance(); // to expected expression
		ast_expr val = computeExpr();
		tuple->append(new NamedTupleEntry{name, tkn, val});

		if (matchNext(","))
		{
			advance(); // to ,
			continue;
		}

		break;
	}

	advance(); // to expected )
	validate(")");

	return tuple;
}

ast_object HydroParser::parseObject()
{
	validate("{");
	ast_object obj = new ObjectInit{currentToken()};
	lex_token tkn;

	if (matchNext(":"))
	{
		advance(); // to :
	}
	else
	{
		while (nextTokenExists())
		{
			advance(); // to expected name
			ast_name name = parseSimpleName();
			advance(); // to expected :
			validate(":");
			tkn = currentToken();
			advance(); // to expected expression
			ast_expr value = computeExpr();
			obj->append(new ObjectInitProperty{name, tkn, value});

			if (matchNext(","))
			{
				advance(); // to ,
				continue;
			}

			break;
		}
	}

	advance(); // to expected }
	validate("}");

	return obj;
}

ast_json HydroParser::parseJson()
{
	validate("{");
	ast_json json = new JsonInit{currentToken()};

	if (!matchNext("}"))
	{
		while (nextTokenExists())
		{
			advance(); // to expected json string
			validate(string_tkn);
			ast_literal prop = new Literal{currentToken()};
			advance(); // to expected :
			validate(":");
			lex_token tkn = currentToken();
			advance(); // to expected JSON expression
			ast_expr val = parseJsonValue();
			json->append(new JsonInitProperty{prop, tkn, val});

			if (matchNext(","))
			{
				advance(); // to ,
				continue;
			}

			break;
		}
	}

	advance(); // to expected }
	validate("}");

	return json;
}

ast_expr HydroParser::parseJsonValue()
{
	if (match(string_tkn))
		return new Literal{currentToken()};
	else if (match("null"))
        return new NullLiteral{currentToken()};
	else if (match(float_tkn) || match(double_tkn) || match(int_tkn))
		return new Literal{currentToken()}; // ex: 1024 or 1024.87
	else if (match("true") || match("false"))
		return new Literal{currentToken()};
	else if (match("-"))
	{
		lex_token tkn = currentToken();
		advance(); // to expected number
		if (match(float_tkn) || match(double_tkn) || match(int_tkn))
			return new UnaryOp{tkn, new Literal{currentToken()}}; // ex: -1.2e-5

		abrupt(currentToken());
	}
	else if (match("{"))
		return parseJson();
	else if (match("["))
		return parseJsonArray();

	// fail
	abrupt();
	return nullptr;
}

ast_array HydroParser::parseJsonArray()
{
	validate("[");
	ast_array arr = new ArrayInit{currentToken()};

	while (nextTokenExists())
	{
		advance(); // to expected JSON value
		arr->append(parseJsonValue());
		if (matchNext(","))
		{
			advance(); // to ,
			continue;
		}
		else
			break;
	}

	advance(); // to expected ]
	validate("]");

	return arr;
}

ast_array HydroParser::parseArray()
{
	validate("[");
	ast_array arr = new ArrayInit{currentToken()};

	if (!matchNext("]"))
	{
		while (nextTokenExists())
		{
			advanceSkip(ln_tkn); // to expected expression (allow linefeed)
			ast_expr expr = computeExpr();
			arr->append(expr);
			if (matchNext(",", ln_tkn))
			{
				advanceSkip(ln_tkn); // to comma (allow linefeed)
				continue;
			}

			break;
		}
	}
    
	advance(); // to expected ] (allow linefeed)
	validate("]");

	return arr;
}

ast_dict HydroParser::parseDict()
{
	validate("[");
	ast_dict dict = new DictInit{currentToken()};

	if (matchNext(":"))
	{
		advanceSkip(ln_tkn); // to :
		advanceSkip(ln_tkn); // to expected ]
		validate("]");
		return dict;
	}

	while (nextTokenExists())
	{
		advanceSkip(ln_tkn); // to expected expression
		ast_expr key = computeExpr();
		advanceSkip(ln_tkn); // to expected :
		validate(":");
		lex_token tkn = currentToken();
		advanceSkip(ln_tkn); // to expected expression
		ast_expr val = computeExpr();
		dict->append(key, tkn, val);

		if (matchNext(",", ln_tkn))
		{
			advance(); // to ,
			continue;
		}

		break;
	}

	advanceSkip(ln_tkn); // to expected ]
	validate("]");

	return dict;
}

ast_stmt HydroParser::parseStmt(bool eos)
{
	skip(ln_tkn); // skip redundant linefeeds
	ast_stmt stmt = nullptr;

	if (specImport())
		stmt = parseImport();
    else if(specPackage())
        stmt = parsePackage();
    else if(specLibrary())
        stmt = parseLibrary();
	else if (specVar()) // includes "const" and "let" declarations
		stmt = parseVar();
	else if (specIf())
		stmt = parseIf();
	else if (specMatch("for"))
		stmt = parseFor();
	else if (specMatch("do"))
		stmt = parseDoWhile();
	else if (specMatch("while"))
		stmt = parseWhile();
	else if (specMatch("continue"))
		stmt = parseContinue();
	else if (specMatch("break"))
		stmt = parseBreak();
	else if (specTry())
		stmt = parseTry();
    else if (assumeThrow())
        stmt = parseThrow();
	else if (assumeFunc())
		stmt = parseFunc();
    else if (assumeAction())
        stmt = parseAction();
	else if (match("return"))
		stmt = parseReturn();
    else if (assumeEtype())
        stmt = parseEtype();
    else if (assumeEvent())
        stmt = parseEvent();
    else if (assumeParam())
        stmt = parseParam();
	else if (assumeClass())
		stmt = parseClass();
	else if (specConstructor())
		stmt = parseConstructor();
	else if (specProperty())
		stmt = parseProperty();
	else if (specMethod())
		stmt = parseMethod();
	else if (match(";"))
	{
		// empty statement
		return new EmptyStmt{}; // skip eos
	}
	else
	{
		// expression statement
		stmt = new ExprStmt{computeExpr()};
	}

	if (eos && nextTokenExists())
	{
		advance(); // to expected end of statement
		parseEos();
	}

	return stmt;
}

void HydroParser::parseEos()
{
	if (!match(ln_tkn) && !isEof() && tokenExists())
		validate(";");
}

ast_import HydroParser::parseImport()
{
	validate("import");
	lex_token tkn = currentToken();
	advance();				 // to expected expression
    Name *name = parseName();
	return new ImportStmt{tkn, name};
}

ast_mod HydroParser::parseModifiers()
{
	ast_mod mod = new Modifier{};

	while (isModifier(tokenVal()))
	{
		SimpleName *m = parseSimpleName();
		mod->addMod(m);
		advance(); // to next token
	}

	return mod;
}

ast_var HydroParser::parseVar()
{
	ast_mod mod = parseModifiers();
	if (!match("let") && !match("const"))
		validate("var");

	lex_token tkn = currentToken();
	advance(); // to expected name
	ast_name name = parseSimpleName();
	ast_htype type = parseTypeDecl();

    ast_expr valueExpr = nullptr;

	if (matchNext("="))
	{
		advance(); // to =
		advance(); // to expected expression
		valueExpr = computeExpr();
	}

	VarSymbol *symbol = new VarSymbol{name, type};
	define(symbol);
	return new VarDecl{tkn, mod, name, type, valueExpr, symbol};
}

ast_typeref HydroParser::parseTypeRef()
{
	if (match(package_tkn))
	{
		ast_package pkg = new PackageSpec{currentToken()};
		advance(); // to expected ::
		validate("::");
		lex_token tkn = currentToken();
		advance(); // to expected name
		return new TypeReference{pkg, tkn, parseSimpleName()};
	}
	else
		return new TypeReference{parseSimpleName()};
}

ast_htype HydroParser::parseTypeSpec()
{
	if (matchNext("["))
	{
		// array specifier
		advance(); // to [
		lex_token tkn = currentToken();
		advance(); // to expected type specifier

		if (matchNext("]"))
		{
			advance(); // to ]
			return new ArraySpec{tkn, new NilSpec{}};
		}

		return new ArraySpec{tkn, parseTypeSpec()};
	}
	else if (matchNext("("))
	{
		// tuple specifier
		ast_tuplespec tup = new TupleSpec(currentToken());

		if (!match(")"))
		{
			do
			{
				advance(); // to expected type specifier
				tup->append(parseTypeSpec());
				if (matchNext(","))
				{
					advance(); // to ,
					continue;
				}

				break;
			} while (nextTokenExists());
		}

		advance(); // to expectd )
		validate(")");

		return tup;
	}
	else if (matchNext("*"))
	{
		advance(); // to *
		return new NilSpec{currentToken()};
	}
	else
	{
		advance(); // to expected type reference
		ast_objectspec obj = new ObjectSpec{parseTypeRef()};

		if (matchNext("|"))
		{
			advance(); // to |

			// specifier has more than one specification
			ast_varspec var = new VariadicSpec{};
			var->append(obj);

			do
			{
				advance(); // to expected type specifier

				if (matchNext(package_tkn) || matchNext(id_tkn))
				{
					advance(); // to expected type reference
					var->append(new ObjectSpec{parseTypeRef()});
				}
			} while (nextTokenExists());

			return var;
		}

		return obj;
	}
}

ast_htype HydroParser::parseTypeDecl()
{
	// optional type specification
	if (matchNext(":"))
	{
		advance(); // to expected :
		return parseTypeSpec();
	}

	return new NilSpec{};
}

ast_block HydroParser::parseBlock()
{
	ast_block blk;
	lex_token tkn = currentToken();

	blk = new Block{tkn};
	validate("{");
    //pushScope(new Scope{});

	if (!matchNext("}", ln_tkn))
	{
		while (nextTokenExists())
		{
			advanceSkip(ln_tkn);				 // to expected statement
			blk->append(parseStmt(false)); // don't check for end of statement (;)
			if (matchNext("}", ln_tkn))
				break;
			else if(!matchNext(ln_tkn))
			{
				//advanceSkip(ln_tkn); // to expected end of statement
                advance();
                parseEos();
			}
            else if(matchNext(";", ln_tkn))
            {
                advanceSkip(ln_tkn);
                parseEos();
            }

            if(matchNext("}", ln_tkn))
                break;
		}
	}

    advanceSkip(ln_tkn); // to expected }
    validate("}");
    //popScope();
    
	return blk;
}

ast_stmt HydroParser::parseBody()
{
	if (match("{"))
		return parseBlock();
	else
		return parseStmt();
}

ast_if HydroParser::parseIf()
{
	validate("if");
	lex_token tkn = currentToken();
	advance(); // to expected expression
	ast_expr cond = computeExpr();
	advanceSkip(ln_tkn); // to expected block
	ast_stmt blk = parseBlock();
	ast_else els = nullptr;

	if (matchNext("else", ln_tkn))
	{
		advanceSkip(ln_tkn); // to else
		lex_token tkn2 = currentToken();
		advanceSkip(ln_tkn); // to expected stmt or block

		if (match("{"))
		{
			// block
			els = new ElseStmt{tkn2, parseBlock()};
		}
		else
		{
			// statement (possibly else if)
			els = new ElseStmt{tkn2, parseStmt(false)};
		}
	}

	return new IfStmt{tkn, cond, blk, els};
}

ast_try HydroParser::parseTry()
{
	lex_token tkn = currentToken();
	validate("try");
	advanceSkip(ln_tkn); // to expected block
	ast_stmt blk = parseBlock();
	ast_try tryStmt = new TryStmt{tkn, blk};

	if (matchNext("catch", ln_tkn))
	{
		while (matchNext("catch", ln_tkn))
		{
			advanceSkip(ln_tkn); // to "catch"
			tryStmt->append(parseCatch());
		}
	}
	else
	{
        // error
		advanceSkip(ln_tkn);
        appendError("Expecting a catch clause after try block.");
	}

	return tryStmt;
}

CatchClause *HydroParser::parseCatch()
{
    validate("catch");
    Token *tkn = currentToken();
    bool trad = false;
    Stmt *body;
    
    if(matchNext("(", ln_tkn))
    {
        advanceSkip(ln_tkn); // to (
        trad = true;
    }
    
    advanceSkip(ln_tkn);
    SimpleName *name = parseSimpleName();
    TypeSpec *type;
    
    if(matchNext(":"))
    {
        advance(); // to expected type
        type = parseTypeSpec();
    }
    else
    {
        // nil type
        type = nullptr;
    }
    
    VarSymbol *symbol = new VarSymbol{name, type};
    define(symbol);
    
    if(trad)
    {
        advanceSkip(ln_tkn); // to expected )
        validate(")");
        advanceSkip(ln_tkn); // to expected body
        body = parseBody();
    }
    else
    {
        advanceSkip(ln_tkn); // to expected block
        body = parseBlock();
    }
    
    return new CatchClause{tkn, name, type, body, symbol};
}

ThrowStmt *HydroParser::parseThrow()
{
    validate("throw");
    Token *tkn = currentToken();
    advance(); // to expected expression
    Expr *expr = computeExpr();
    return new ThrowStmt{tkn, expr};
}

ast_continue HydroParser::parseContinue()
{
	validate("continue");
	lex_token tkn = currentToken();

	if (!matchNext(ln_tkn) && !matchNext(";"))
	{
		advance(); // to expr
		return new ContinueStmt{tkn, computeExpr()};
	}

	return new ContinueStmt{tkn};
}

ast_break HydroParser::parseBreak()
{
	validate("break");
	lex_token tkn = currentToken();

	if (!matchNext(ln_tkn) && !matchNext(";"))
	{
		advance(); // to expected expr
		return new BreakStmt{tkn, computeExpr()};
	}

	return new BreakStmt{tkn};
}

ast_for HydroParser::parseFor()
{
	validate("for");
	lex_token tkn = currentToken();
    bool traditional = false;
    ast_stmt body;
    
    if(matchNext("(", ln_tkn))
    {
        advanceSkip(ln_tkn);
        traditional = true;
    }
    
    ast_expr a; // iterator
    ast_expr b; // condition
    ast_expr c; // 
    
    // parse initializer expression
    if(matchNext(";", ln_tkn))
    {
        // empty
        advanceSkip(ln_tkn); // to expected ;
        validate(";");
        a = emptyExpr();
    }
    else
    {
        // expression
        advanceSkip(ln_tkn); // to expected expr
        a = computeExpr();
        advanceSkip(ln_tkn); // to expected ;
        validate(";");
    }
    
    // parse condition expression
    if(matchNext(";", ln_tkn))
    {
        // empty
        advanceSkip(ln_tkn); // to expected ;
        validate(";");
        b = emptyExpr();
    }
    else
    {
        // expression
        advanceSkip(ln_tkn); // to expected expr
        b = computeExpr();
        advanceSkip(ln_tkn); // to expected ;
        validate(";");
    }
    
    // parse iterator expression
    if(traditional && matchNext(")", ln_tkn))
    {
        // empty
        c = emptyExpr();
    }
    else if(!traditional && matchNext("{", ln_tkn))
    {
        // empty
        c = emptyExpr();
    }
    else
    {
        // expression
        advanceSkip(ln_tkn);
        c = computeExpr();
    }
    
	// parse body
    if(traditional)
    {
        advanceSkip(ln_tkn); // to expected )
        validate(")");
        advanceSkip(ln_tkn); // to expected body
        body = parseBody();
    }
    else
    {
        advanceSkip(ln_tkn); // to expected block
        body = parseBlock();
    }
    
	return new ForStmt{tkn, a, b, c, body};
}

ast_do HydroParser::parseDoWhile()
{
	validate("do");
	lex_token tkn = currentToken();
	advanceSkip(ln_tkn); // to expected block
	ast_stmt block = parseBlock();
	advanceSkip(ln_tkn); // to expected while
	validate("while");
	advanceSkip(ln_tkn); // to expected expression
	bool retro = false;  // retro style for (...) {}
	if (matchNext("(", ln_tkn))
	{
		advanceSkip(ln_tkn); // to (
		retro = true;
	}

	ast_expr cond = computeExpr();

	if (retro)
	{
		advanceSkip(ln_tkn);
		validate(")");
	}

	return new DoStmt{tkn, block, cond};
}

ast_while HydroParser::parseWhile()
{
	validate("while");
	lex_token tkn = currentToken();
	advance(); // to expected expression

	ast_expr cond = computeExpr();
	advanceSkip(ln_tkn); // to expected block
	ast_stmt block = parseBlock();
	return new WhileStmt{tkn, cond, block};
}

PackageDecl *HydroParser::parsePackage()
{
    validate("package");
    Token *tkn = currentToken();
    Name *name = nullptr;
    PackageSymbol *symbol;
    Block *empliedBlock = new Block{};
    
    if(matchNext(id_tkn))
    {
        advance(); // to name
        name = parseName();
    }
    
    // pop file scope
    //popScope(); // use when file ast is implemented
    
    if(name)
    {
        // add tree to package hierarchy
        symbol = buildPackageTree(name);
        assert(symbol);
    }
    else
    {
        // load symbol
        symbol = dynamic_cast<PackageSymbol *>(_globalScope->resolve("global"));
    }
    
    define(symbol);
    pushScope(symbol->ownScope());
    Stmt *stmt;
    
    while(nextTokenExists())
    {
        advanceSkip(ln_tkn); // to expected statement

        if (!match(eof_tkn) && tokenExists())
        {
            stmt = parseStmt(true);
            empliedBlock->append(stmt);
        }
    }
    
    popScope();
    return new PackageDecl{tkn, symbol, empliedBlock};
}

LibraryDecl *HydroParser::parseLibrary()
{
    validate("library");
    Token *tkn = currentToken();
    SimpleName *name = nullptr;
    Block *empliedBlock = new Block{};
    advance(); // to name
    name = parseSimpleName();
    
    // pop file scope
    //popScope(); // use when file ast is implemented
    
    LibrarySymbol *symbol = new LibrarySymbol{name, new Scope("[library " + name->value() + "]")};
    define(symbol);
    pushScope(symbol->ownScope());
    Stmt *stmt;
    
    while(nextTokenExists())
    {
        skip(ln_tkn);

        if (!match(eof_tkn) && tokenExists())
        {
            stmt = parseStmt(true);
            empliedBlock->append(stmt);
        }
    }
    
    popScope();
    return new LibraryDecl{tkn, symbol, empliedBlock};
}

ast_func HydroParser::parseFunc()
{
	ast_mod mod = parseModifiers();
	validate("func");
	lex_token tkn = currentToken();
	advance(); // to expected name
	ast_name name = parseSimpleName();
    
	// arguments
	advance(); // to expected args

    // package exists?
    PackageSymbol *pkg = loadEnclosingPackage();
    
    // symbol
    Scope *ownScope = new Scope{name->value() + "()"};
    FuncSymbol *symbol = new FuncSymbol{mod, name, ownScope, nullptr, pkg};
    define(symbol);
    pushScope(ownScope);
    
	ast_fargs fargs = parseFuncArgs();
    
	ast_htype returnType = parseTypeDecl();
	ast_stmt block = nullptr;
    
	// block exists ?
	if (matchNext("{", ln_tkn))
	{
		advanceSkip(ln_tkn); // to expected {
		block = parseBlock();
	}
    
	popScope();
	return new FuncDecl{tkn, symbol, fargs, returnType, block, pkg};
}

ast_fargs HydroParser::parseFuncArgs()
{
	validate("(");
	ast_fargs fargs = new FuncArgs{currentToken()};
	bool variadic = false;

	if (!matchNext(")"))
	{
		while (nextTokenExists())
		{
			advanceSkip(ln_tkn); // to expected name or ... (allow linefeed)

			if (match("..."))
			{
				advance(); // to expected name
				variadic = true;
			}

			ast_name argName = parseSimpleName();
			skip(ln_tkn);
			ast_htype argType = parseTypeDecl();
			ast_expr defaultValue = nullptr;

			// default value
			if (matchNext("=", ln_tkn))
			{
				advanceSkip(ln_tkn); // to = (allow linefeed)
				advanceSkip(ln_tkn); // to expected expression (allow linefeed)
				defaultValue = computeExpr();
			}
            
            FuncArgSymbol *symbol = new FuncArgSymbol{argName, argType};
            define(symbol);
			fargs->append(new Farg{argName, argType, defaultValue, variadic, symbol});

			// next argument
			if (matchNext(",", ln_tkn))
			{
				advanceSkip(ln_tkn); // to comma (allow linefeed)
				continue;
			}

			// end
			break;
		}
	}

	advanceSkip(ln_tkn); // to expected ) (allow linefeed)
	validate(")");
	return fargs;
}

ActDecl *HydroParser::parseAction()
{
    ast_mod mod = parseModifiers();
    validate("act");
    Token *token = currentToken();
    advance(); // to expected name
    SimpleName *name = parseSimpleName();
    advance(); // to expected [
    validate("[");
    advance(); // to expected name
    Name *context = parseName();
    advance(); // to expected ]
    
    PackageSymbol *package = loadEnclosingPackage();
    Scope *ownScope = new Scope{_currentScope};
    ActSymbol *symbol = new ActSymbol{mod, name, context, ownScope, package};
    define(symbol);
    pushScope(ownScope);
    advanceSkip(ln_tkn); // to expected block
    Block *body = parseBlock();
    popScope();
    return new ActDecl{token, mod, name, context, body, package, symbol};
}

ast_return HydroParser::parseReturn()
{
	validate("return");
	lex_token tkn = currentToken();

	if (assumeReturn() && assumeOpnd())
	{
		advance(); // to expected expr
		return new ReturnStmt{tkn, computeExpr()};
	}

	return new ReturnStmt{tkn};
}

EtypeDecl *HydroParser::parseEtype()
{
    ast_mod mod = parseModifiers();
    validate("etype");
    Token *token = currentToken();
    advance(); // to expected name
    Name *name = parseSimpleName();
    EtypeSymbol *symbol = new EtypeSymbol{mod, name};
    define(symbol);
    return new EtypeDecl{token, mod, name};
}

EventDecl *HydroParser::parseEvent()
{
    ast_mod mod = parseModifiers();
    validate("event");
    Token *token = currentToken();
    advance(); // to expected name
    Name *name = parseSimpleName();
    std::list<Name *> types;
    
    if(matchNext("<"))
    {
        advance(); // to < (extends)
        
        do
        {
            advance(); // to expected type
            types.push_back(parseName());

            if (matchNext(","))
            {
                advance(); // to ,
                continue;
            }

            break;
        } while (nextTokenExists());
    }
    
    PackageSymbol *package = loadEnclosingPackage();
    Scope *ownScope = new Scope{_currentScope};
    EventSymbol *symbol = new EventSymbol{mod, name, package};
    advanceSkip(ln_tkn); // to expected block
    define(symbol);
    pushScope(ownScope);
    Block *body = parseBlock();
    popScope();
    EventDecl *event = new EventDecl{token, mod, name, body, symbol, package};
    
    // append types
    for(auto type : types)
        event->appendType(type);
    
    return event;
}

ParamDecl *HydroParser::parseParam()
{
    ast_mod mod = parseModifiers();
    validate("param");
    Token *token = currentToken();
    advance(); // to expected name
    Name *name = parseSimpleName();
    ast_htype type = parseTypeDecl();
    ast_expr valueExpr = nullptr;

    if (matchNext("="))
    {
        advance(); // to =
        advance(); // to expected expression
        valueExpr = computeExpr();
    }
    
    ParamSymbol *symbol = new ParamSymbol{mod, name, type};
    define(symbol);
    return new ParamDecl{token, mod, name, type, symbol, valueExpr};
}

ast_class HydroParser::parseClass()
{
	ast_mod mod = parseModifiers();
	validate("class");
	lex_token tkn = currentToken();
	advance(); // to expected name
	Name *name = parseSimpleName();
    
    PackageSymbol *pkg = loadEnclosingPackage();
    
	std::list<Name *> types;

	// super class and interfaces
	if (matchNext("<"))
	{
		advance(); // to < (extends and implements)

		do
		{
			advance(); // to expected type
            types.push_back(parseName());

			if (matchNext(","))
			{
				advance(); // to ,
				continue;
			}

			break;
		} while (nextTokenExists());
    }
    
	advanceSkip(ln_tkn); // to expected block
    Scope *ownScope = new Scope{"[class" + name->value() + "]", _currentScope};
    ClassSymbol *symbol = new ClassSymbol{mod, name, ownScope, pkg};
    define(symbol);
    pushScope(ownScope);
	ClassDecl *clss = new ClassDecl{tkn, mod, name, parseBlock(), pkg, symbol};
    popScope();
    
    // append types
    for(auto type : types)
        clss->appendType(type);
    
    return clss;
}

ast_construct HydroParser::parseConstructor()
{
	ast_mod mod = parseModifiers();
	validate("constructor");
	lex_token tkn = currentToken();
	advance(); // to expected args
    
    
    ClassSymbol *owner = getEnclosingClass();
    Scope *ownScope = new Scope{"::constructor()"};
    ConstructorSymbol *symbol = new ConstructorSymbol{mod, ownScope, owner};
    define(symbol);
    pushScope(ownScope);

	ast_fargs fargs = parseFuncArgs();
	ast_htype returnType = parseTypeDecl(); // erroneous if exists
	ast_stmt block = nullptr;
    
    if(!owner)
    {
        // illegal!
        appendError("Illegal constructor declaration outside of class context.", tkn);
    }
    
	if (matchNext("{", ln_tkn))
	{
		advanceSkip(ln_tkn); // to constructor body
		block = parseBlock();
	}
    
    popScope();
	return new ConstructorDecl{tkn, mod, fargs, returnType, block, symbol};
}

ast_property HydroParser::parseProperty()
{
	ast_mod mod = parseModifiers();
	validate("prop");
	lex_token tkn = currentToken();
	advance(); // to expected name
	SimpleName *name = parseSimpleName();
	TypeSpec *type = parseTypeDecl();
	Expr *initVal = nullptr;
    
    Scope *ownScope = nullptr;
    PropertySymbol *symbol;
    
    ClassSymbol *owner = getEnclosingClass();
    
    if(!owner)
    {
        // error
        appendError("Property declaration outside of class context.", tkn);
    }
    
	if (matchNext("="))
	{
        symbol = new PropertySymbol{mod, name, type, owner, ownScope};
        define(symbol);
		advance(); // to =
		advance(); // to expected expression
		initVal = computeExpr();
		return new PropertyDecl{tkn, mod, name, type, initVal, symbol};
	}
	else if (matchNext("{", ln_tkn))
	{
		advanceSkip(ln_tkn); // to block
        Block *block = new Block{currentToken()};
		GetterDecl *getter = nullptr;
		SetterDecl *setter = nullptr;
        Scope *ownScope = new Scope{name->value(), _currentScope};
        PropertySymbol *symbol = new PropertySymbol{mod, name, type, owner, ownScope};
        define(symbol);
        pushScope(ownScope);
        
		while (nextTokenExists())
		{
			advanceSkip(ln_tkn); // to expected getter or settter declaration

			if (specGetter())
			{
				if (getter)
				{
                    // error
					appendError("Redefinition of getter on property '" + name->value() + "'.", tkn);
				}
                
				getter = parseGetter();
                block->append(getter);
			}
			else if (specSetter())
			{
                if(setter)
                {
                    // error
                    appendError("Redefinition of setter on property '" + name->value() + "'.", tkn);
                }
                
				setter = parseSetter();
                block->append(setter);
			}
            else if(match("}"))
            {
                // end
                validate("}");
                break;
            }
            else
            {
                // error
                abrupt();
                break;
            }
		}
        
        popScope();
		return new PropertyDecl{tkn, mod, name, type, block, symbol};
	}
	else
	{
        symbol = new PropertySymbol{mod, name, type, owner, ownScope};
        define(symbol);
		return new PropertyDecl{tkn, mod, name, type, initVal, symbol};
	}
}

ast_getter HydroParser::parseGetter()
{
	ast_mod mod = parseModifiers();
	validate("[");
	advance();
	validate("get");
	lex_token tkn = currentToken();
	advance();
	validate("]");
	ast_fargs args = nullptr;
	if (matchNext("("))
	{
		advance(); // to (
		args = parseFuncArgs();
	}

	ast_block block = nullptr;
	if (matchNext("{", ln_tkn))
	{
		advanceSkip(ln_tkn);
		block = parseBlock();
	}
	return new GetterDecl(tkn, mod, args, block);
}

ast_setter HydroParser::parseSetter()
{
	ast_mod mod = parseModifiers();
	validate("[");
	advance();
	validate("set");
	lex_token tkn = currentToken();
	advance();
	validate("]");
	ast_fargs args = nullptr;
	if (matchNext("("))
	{
		advance(); // to (
		args = parseFuncArgs();
	}
	ast_block block = nullptr;
	if (matchNext("{", ln_tkn))
	{
		advanceSkip(ln_tkn);
		block = parseBlock();
	}
	return new SetterDecl(tkn, mod, args, block);
}

void HydroParser::reset()
{
	Parser::reset();
	_parsing = false;
}

ast_method HydroParser::parseMethod()
{
	ast_mod mod = parseModifiers();
	validate("meth");
	lex_token tkn = currentToken();
	advance(); // to expected name
	ast_name name = parseSimpleName();
    
    // symbol
    Scope *ownScope = new Scope{name->value() + "()"};
    MethodSymbol *symbol = new MethodSymbol{mod, name, ownScope, nullptr};
    define(symbol);
    pushScope(ownScope);

	// arguments
	advance(); // to expected args

	ast_fargs fargs = parseFuncArgs();
	ast_htype returnType = parseTypeDecl();
	ast_stmt block = nullptr;

	// block exists ?
	if (matchNext("{", ln_tkn))
	{
		advanceSkip(ln_tkn); // to expected {
		block = parseBlock();
	}
    
	popScope();
	return new MethodDecl{tkn, mod, name, fargs, returnType, block};
}

PackageSymbol *HydroParser::buildPackageTree(Name *name, Scope *scope)
{
    Symbol *symbol;
    PackageSymbol *pkg;
    PackageSymbol *parentPkg = nullptr;
    
    if(!scope)
        scope = _globalScope;
    
    if((pkg = resolvePackage(name)))
        return pkg;
    
    if(QualifiedName *qname = dynamic_cast<QualifiedName *>(name))
    {
        // build and resolve qualifier (recursively)
        parentPkg = buildPackageTree(qname->qualifier());
        
        // get new scope
        scope = parentPkg->ownScope();
        
        // get simple name
        name = qname->name();
    }
    
    // resolve package using simple name
    symbol = scope->get(name->value());
    
    if((pkg = dynamic_cast<PackageSymbol *>(symbol)))
    {
        // already exists
        return pkg;
    }
    else if(symbol)
    {
        // error!
        throw 1;
    }
    
    // load global package
    if(!parentPkg)
    {
        parentPkg = dynamic_cast<PackageSymbol *>(_globalScope->resolve(""));
        assert(parentPkg);
    }
    
    // create symbol with own scope
    pkg = new PackageSymbol{name, new Scope{"@" + name->value(), scope}, parentPkg};
    assert(parentPkg->ownScope()->owner());
    scope->define(pkg);
    return pkg;
}

PackageSymbol *HydroParser::resolvePackage(Name *name, Scope *scopeToUse)
{
    if(!scopeToUse)
        scopeToUse = _globalScope;
    return dynamic_cast<PackageSymbol *>(scopeToUse->resolve(name->value())); // find a package or fail
}

PackageSymbol *HydroParser::loadEnclosingPackage()
{
    return _currentScope ? dynamic_cast<PackageSymbol *>(_currentScope->owner()) : nullptr;
}

PackageSymbol *HydroParser::loadParentPackage(Name *packageName)
{
    PackageSymbol *pkg = buildPackageTree(packageName);
    
    // try
    return dynamic_cast<PackageSymbol *>(pkg->parent());
}

ClassSymbol *HydroParser::getEnclosingClass()
{
    Scope *scope = _currentScope;
    
    while (scope)
    {
        Symbol *owner = scope->owner();
        
        if(owner)
        {
            if(ClassSymbol *clss = dynamic_cast<ClassSymbol *>(owner))
                return clss; // success!
            
            // fail
            break;
        }
        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

Symbol *HydroParser::getOwnerOf(Scope *scope)
{
    while(scope)
    {
        if(Symbol *owner = scope->owner())
            return owner;
        
        // try again
        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

void HydroParser::import(Name *name)
{
}

} // namespace hydro
