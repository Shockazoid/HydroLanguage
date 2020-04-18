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

#ifndef __h3o_HydroAssemblerLexer__
#define __h3o_HydroAssemblerLexer__

#include "../../parser/Lexer.hpp"

namespace hydro
{

static const std::regex asm_directive{"^\\.[a-zA-Z0-9]+$"};

class HydroAssemblerLexer : public Lexer
{
private:
    bool predictHex()
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

    bool predictBin()
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

    bool predictOct()
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

    bool predictInt(bool bypassCheck = true)
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

    bool predictShort()
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

    bool predictLong()
    {
        specStart();

        if (predictInt() && specPrev() != 'i' && specPrev() != 'I')
        {
            if (specMatch('l') || specMatch('L'))
                return specSuccess();
        }

        return specFail();
    }

    bool predictUint(bool bypassCheck = true)
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

    bool predictUshort()
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

    bool predictUlong()
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

    bool predictExp()
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

    bool predictFloat()
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

    bool predictDouble(bool bypassCheck = true)
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

    bool validateNum()
    {
        specStart();

        while (specMatch(lexer_alphanum_underscore))
            return specFail();

        return specSuccess();
    }

    lex_token id()
    {
        while (matchBufferWithNext(id_tkn))
            consume();

        return flush(id_tkn);
    }

    lex_token hex()
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

    lex_token bin()
    {
        consume(); // eat 0
        consume(); // eat b

        while (matchBufferWithNext(lexer_binary))
            consume();

        return flush(bin_tkn);
    }

    lex_token oct()
    {
        consume(); // eat 0

        while (matchBufferWithNext(lexer_octal))
            consume();

        return flush(oct_tkn);
    }

    lex_token u16()
    {
        consume(); // eat first digit

        while (matchBufferWithNext(lexer_digit))
            consume();

        consume(); // eat u
        consume(); // eat s

        return flush(ushort_tkn);
    }

    lex_token i16()
    {
        consume(); // eat first digit

        while (matchBufferWithNext(lexer_digit))
            consume();

        consume(); // eat s

        return flush(short_tkn);
    }

    lex_token u32()
    {
        consume(); // eat first digit

        while (matchBufferWithNext(lexer_digit))
            consume();

        consume(); // eat u
        if (matchNext('i') || matchNext('i'))
            consume(); // eat optional i

        return flush(uint_tkn);
    }

    lex_token i32()
    {
        consume(); // eat first digit

        while (matchBufferWithNext(lexer_digit))
            consume(); // eat digits

        if (matchNext('i') || matchNext('I'))
            consume(); // eat optional i

        return flush(int_tkn);
    }

    lex_token u64()
    {
        consume(); // eat first digit

        while (matchBufferWithNext(lexer_digit))
            consume();

        consume(); // eat u
        consume(); // eat l

        return flush(ulong_tkn);
    }

    lex_token i64()
    {
        consume(); // eat first digit

        while (matchBufferWithNext(lexer_digit))
            consume();

        consume(); // eat l

        return flush(long_tkn);
    }

    lex_token exp()
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

    lex_token flt()
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

    lex_token dbl()
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

    lex_token chr()
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
                    appendError("Unknown escape sequence '" + std::string{1, next()} + "'.", line(), position());
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

    lex_token string()
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
                        appendError("Unknown escape sequence '" + std::string{1, next()} + "'.", line(), position());
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
            _compiler->appendError("Unterminated string literal.", source()->location(), line(), position());

        return flush(string_tkn);
    }

    lex_token dir() // .directive
    {
        consume(); // eat .

        do
            consume();
        while (matchBufferWithNext(asm_directive));

        return flush(directive_tkn);
    }
    lex_token pkg()
    {
        consume(); // eat @

        do
        {
            if (matchNext('/') && matchLa(lexer_alphanum_underscore, 2))
                consume();
            else if (matchNext(lexer_alphanum_underscore))
                consume();
            else
                break;
        } while (nextExists());

        return flush(package_tkn);
    }

    lex_token com()
    {
        do
            consume(); // eat ; and all characters before line break or eof
        while (nextExists() && !matchNext('\n'));

        return flush(scomment_tkn);
    }

protected:
    virtual lex_token readNext() override
    {
        if (matchNext(id_tkn))
            return id();
        else if (isEof(1))
        {
            consume();
            return flush(eof_tkn);
        }
        else if (matchNext('\n'))
        {
            consume();
            return flush(ln_tkn);
        }
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
            appendError("Invalid syntax for assumed numerical literal.", line(), position());
        }
        else if (matchNext('\'')) // assume char
            return chr();
        else if (matchNext('"')) // assume string literal
            return string();
        else if (matchNext(';'))
            return com(); // single line comment
        else if (lex_token token = devour(token_tkn))
            return token;
        else if (lex_token op = devour(op_tkn))
            return op;
        if (matchNext('.') && matchLa(lexer_alpha, 2))
            return dir();
        else if (matchNext('@') && matchLa(lexer_alphanum_underscore, 2))
            return pkg();

        consume();
        return flush(token_tkn);
    }

public:
    HydroAssemblerLexer(Compiler *compiler) : Lexer{compiler}
    {
        registerTokenPattern(id_tkn, lexer_identifier);
    }
    virtual ~HydroAssemblerLexer() {}
};

} // namespace hydro

#endif /* __h3o_HydroAssemblerLexer__ */
