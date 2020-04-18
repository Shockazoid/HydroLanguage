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

#include "HydroAssemblerParser.hpp"
#include "../errors.hpp"

namespace hydro
{

HydroAssemblerParser::HydroAssemblerParser(Compiler *compiler) : BytecodeAssembler{new HydroAssemblerLexer{compiler}} {}

HydroAssemblerParser::~HydroAssemblerParser() {}

void HydroAssemblerParser::push(AssemblyNode *node)
{
    _stack.push(node);
}

void HydroAssemblerParser::pop()
{
    if(!_stack.empty())
        _stack.pop();
}

void *HydroAssemblerParser::getEnclosingFunc()
{
    
    // fail
    return nullptr;
}

void HydroAssemblerParser::emit(AssemblyNode *node)
{
    _stack.top()->appendChild(node);
}

void HydroAssemblerParser::emit(hvm_isa_opcodes instr)
{
    emit(new BasicInstrNode{instr});
}

void HydroAssemblerParser::emit(hvm_isa_opcodes instr, int32_t opnd1)
{
    emit(new BasicInstrNode{instr, opnd1});
}

void HydroAssemblerParser::emit(hvm_isa_opcodes instr, int32_t opnd1, int32_t opnd2)
{
    emit(new BasicInstrNode{instr, opnd1, opnd2});
}

void HydroAssemblerParser::emit(hvm_isa_opcodes instr, int32_t opnd1, int32_t opnd2, int32_t opnd3)
{
    emit(new BasicInstrNode{instr, opnd1, opnd2, opnd3});
}

uint32_t HydroAssemblerParser::registerName(std::string name)
{

    uint32_t index = 0;

    for (std::string n : _names)
    {
        if (n == name)
            return index;

        index++;
    }

    _names.push_back(name);

    return index;
}

uint32_t HydroAssemblerParser::registerConstant(hvalue constant)
{
    uint32_t index = 0;

    for (hvalue c : _constPool)
    {
        if (c.type() == constant.type() && c == constant)
            return index;
        else if (is_string(c) && is_string(constant))
        {
            // compare strings
            HString *s1 = c;
            HString *s2 = constant;
            if (s1->toString() == s2->toString())
                return index;
        }
        index++;
    }

    _constPool.push_back(constant);
    return (uint32_t)_constPool.size() - 1;
}

LabelNode *HydroAssemblerParser::defineLabel(std::string label)
{
    for (LabelNode *lab : _labels)
    {
        if (lab->value() == label)
        {
            if (lab->isDefined())
            {
                // error: already defined
                return nullptr;
            }
            else
            {
                // define forward referenced label
                lab->define();
                return lab;
            }
        }
    }

    // create and define
    LabelNode *lab = new LabelNode{label};
    lab->define();
    _labels.push_back(lab);
    return lab;
}

LabelNode *HydroAssemblerParser::getLabel(std::string label)
{
    for (LabelNode *l : _labels)
        if (l->value() == label)
            return l;

    _labels.push_back(new LabelNode{label});
    return _labels.back();
}


void HydroAssemblerParser::onParseInit()
{
    
}

void HydroAssemblerParser::parseConst()
{
    if (match(int_tkn))
        parseIntConst();
    else if (match(string_tkn))
        parseStringConst();
    else if (match(float_tkn))
        parseFloatConst();
    else if (match(double_tkn))
        parseDoubleConst();
    else if (match(char_tkn))
        parseCharConst();
    else if (match(uint_tkn))
        parseUintConst();
    else if (match(short_tkn))
        parseShortConst();
    else if (match(ushort_tkn))
        parseUshortConst();
    else if (match(long_tkn))
        parseLongConst();
    else if (match(ulong_tkn))
        parseUlongConst();
    
    else
        abrupt();
}

void HydroAssemblerParser::parseShortConst()
{
    validate(short_tkn);
    emit(push_instr, registerConstant((int16_t)std::stoi(tokenVal()))); // TODO string to short
}

void HydroAssemblerParser::parseUshortConst()
{
    validate(ushort_tkn);
    emit(push_instr, registerConstant((uint16_t)std::stoi(tokenVal()))); // TODO string to ushort
}

void HydroAssemblerParser::parseIntConst()
{
    validate(int_tkn);
    emit(push_instr, registerConstant(std::stoi(tokenVal())));
}

void HydroAssemblerParser::parseUintConst()
{
    validate(uint_tkn);
    uint32_t i = (uint32_t)std::stoi(tokenVal()); // TODO string to uint
    emit(push_instr, registerConstant(i));
}

void HydroAssemblerParser::parseLongConst()
{
    validate(long_tkn);
    emit(push_instr, registerConstant((int64_t)std::stol(tokenVal())));
}

void HydroAssemblerParser::parseUlongConst()
{
    validate(ulong_tkn);
    emit(push_instr, registerConstant((uint64_t)std::stol(tokenVal())));
}

void HydroAssemblerParser::parseStringConst()
{
    emit(push_instr, registerConstant(readString())); // TODO push string pointer
}

void HydroAssemblerParser::parseFloatConst()
{
    validate(float_tkn);
    emit(push_instr, registerConstant(std::stof(tokenVal())));
}

void HydroAssemblerParser::parseDoubleConst()
{
    validate(double_tkn);
    emit(push_instr, registerConstant(std::stod(tokenVal())));
}

void HydroAssemblerParser::parseCharConst()
{
    validate(char_tkn);
    std::string val = tokenVal();

    // strip quotes
    val = val.substr(1, val.length() - 1);

    char ch = '\0';

    // escape sequence?
    if (val[0] == '\\')
    {
        switch (val[1])
        {
        case '0':
        {
            ch = '\0';
            break;
        }
        case '\\':
        {
            ch = '\\';
            break;
        }
        case '/':
        {
            ch = '/';
            break;
        }
        case '\'':
        {
            ch = '\'';
            break;
        }
        case 'n':
        {
            ch = '\n';
            break;
        }
        case 't':
        {
            ch = '\t';
            break;
        }
        case 'f':
        {
            ch = '\f';
            break;
        }
        case 'r':
        {
            ch = '\r';
            break;
        }
        case '"':
        {
            ch = '\"';
            break;
        }
        case 'u':
        {
            // TODO unicode
            break;
        }
        case 'U':
        {
            // TODO unicode
            break;
        }
        default:
        {
            appendError(HVM_COMPILE_ERROR_UNKNOWN_ESCAPE_SEQUENCE);
            abort();
            break;
        }
        }
    }
    else
    {
        // get char
        ch = val[0];
    }

    emit(push_instr, registerConstant(ch));
}

int32_t HydroAssemblerParser::readInt()
{
    validate(int_tkn);
    return std::stoi(tokenVal());
}

std::string HydroAssemblerParser::readString()
{
    validate(string_tkn);
    std::string tkn = tokenVal();
    std::string val = "";

    for (uint32_t i = 1; i < tkn.length() - 1; i++)
    {
        if (tkn[i] == '\\')
        {
            // escape sequence
            i++;
            switch (tkn[i])
            {
            case '\\': // reverse solidus
            {
                val += '\\';
                break;
            }
            case '/': // solidus
            {
                val += '/';
                break;
            }
            case '"': // quotation mark
            {
                val += '"';
                break;
            }
            case 'n': // linefeed
            {
                val += '\n';
                break;
            }
            case 'f': // formfeed
            {
                val += '\f';
                break;
            }
            case 't': // horizontal tab
            {
                val += '\t';
                break;
            }
            case 'r': // carriage return
            {
                val += '\r';
                break;
            }
            case 'b': // backspace
            {
                val += '\b';
                break;
            }
            case 'u': // unicode
            {
                i += 4;
                // TODO decode unicode
                val += '1';
                break;
            }
            case 'U': // unicode
            {
                i += 4;
                // TODO decode unicode
                val += '1';
                break;
            }
            default:
            {
                // lexer checks for unknown escape sequences
                break;
            }
            }
        }
        else
            val += tkn[i];
    }

    return val;
}

void HydroAssemblerParser::parseInstr()
{
    
}

void HydroAssemblerParser::parseNext()
{
    skip(ln_tkn);
    
    if(!tokenExists())
        return; // end of file
    
    if(match(directive_tkn))
    {
        
    }
    else if(match(id_tkn))
    {
        if(matchNext(":"))
        {
            // label
        }
        else
        {
            parseInstr();
        }
    }
    else
    {
        abrupt();
    }
}

} // namespace hydro
