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

#include "H3oAssembler.hpp"
#include "../vm/HvmEnv.hpp"
#include "../vm/modifiers.hpp"

namespace hydro
{

H3oAssembler::H3oAssembler(HydroCompiler *compiler, Runtime *runtime) : BytecodeAssembler{new H3oAssemblerLexer{compiler}}, _result{}, _names{}, _constPool{}, _types{}, _blocks{}, _stack{}, _runtime{runtime}
{

	_env = compiler->env();
	ignore(ws_tkn);	 // ignore whitespace
	ignore(scomment_tkn); // ignore comments

	// stack manipulation and call frame
	registerOpcode("pop", pop_instr);
	registerOpcode("dup", dup_instr);
	registerOpcode("swp", swp_instr);
	registerOpcode("swap", swp_instr); // alias
	registerOpcode("jmp", jmp_instr);
	registerOpcode("br", jmp_instr); // alias
	registerOpcode("brt", brt_instr);
	registerOpcode("brf", brf_instr);
	registerOpcode("hlt", hlt_instr);
	registerOpcode("halt", hlt_instr); // alias
	registerOpcode("ret", ret_instr);

	// constants
	registerOpcode("push", push_instr);
	registerOpcode("nil", null_instr);
	registerOpcode("null", null_instr); // alias
	registerOpcode("und", undefined_instr);
	registerOpcode("undefined", undefined_instr); // alias
	registerOpcode("tru", true_instr);
	registerOpcode("true", true_instr); // alias
	registerOpcode("fls", false_instr);
	registerOpcode("false", false_instr); // alias
	registerOpcode("nan", nan_instr);
	registerOpcode("inf", inf_instr);
	registerOpcode("infinity", inf_instr); // alias
	registerOpcode("this", this_instr);

	// initializers
	registerOpcode("object", object_instr);
	registerOpcode("objectinit", objectinit_instr);
	registerOpcode("list", list_instr);
	registerOpcode("listpush", listpush);
	registerOpcode("json", json_instr);
	registerOpcode("jsoninit", jsoninit_instr);
	registerOpcode("dict", dict_instr);
	registerOpcode("dictinit", dictinit_instr);
	registerOpcode("xml", xml_instr);
	registerOpcode("regex", rgx_instr);
    registerOpcode("rload", rload_instr);
    
	// branching
	registerOpcode("br", jmp_instr);
	registerOpcode("jmp", jmp_instr);
	registerOpcode("brt", brt_instr);
	registerOpcode("brf", brf_instr);

	// arithmetic
	registerOpcode("pos", pos_instr);
	registerOpcode("neg", neg_instr);
	registerOpcode("exp", exp_instr);
	registerOpcode("mul", mul_instr);
	registerOpcode("div", div_instr);
	registerOpcode("mod", mod_instr);
	registerOpcode("add", add_instr);
	registerOpcode("sub", sub_instr);
    registerOpcode("inc", inc_instr);
    registerOpcode("incproperty", incproperty_instr);
    registerOpcode("incindex", incindex_instr);
    registerOpcode("dec", dec_instr);
    registerOpcode("decproperty", decproperty_instr);
    registerOpcode("decindex", decindex_instr);

	// logical
	registerOpcode("leq", leq_instr);
	registerOpcode("liq", liq_instr);
	registerOpcode("seq", seq_instr);
	registerOpcode("siq", siq_instr);
	registerOpcode("lnt", lnt_instr);
	registerOpcode("lnd", lnd_instr);
	registerOpcode("lor", lor_instr);
	registerOpcode("gt", gt_instr);
	registerOpcode("gte", gte_instr);
	registerOpcode("lt", lt_instr);
	registerOpcode("lte", lte_instr);

	// bitwise
	registerOpcode("bnt", bnt_instr);
	registerOpcode("bnd", bnd_instr);
	registerOpcode("bor", bor_instr);
	registerOpcode("bxr", bxr_instr);
	registerOpcode("brs", brs_instr);
	registerOpcode("bls", bls_instr);

	// memory access
	registerOpcode("store", store_instr);
	registerOpcode("load", load_instr);
	registerOpcode("setproperty", setproperty_instr);
	registerOpcode("getproperty", getproperty_instr);
	registerOpcode("setstatic", setstatic_instr);
	registerOpcode("getstatic", getstatic_instr);
	registerOpcode("gstore", gstore_instr);
	registerOpcode("gload", gload_instr);
	registerOpcode("setindex", setindex_instr);
	registerOpcode("getindex", getindex_instr);
	registerOpcode("ustore", ustore_instr);
	registerOpcode("uload", uload_instr);
	registerOpcode("storex", storex_instr);
	registerOpcode("loadx", loadx_instr);
	registerOpcode("setpropertyx", setpropertyx_instr);
	registerOpcode("getpropertyx", getpropertyx_instr);
	registerOpcode("gstorex", gstorex_instr);
	registerOpcode("gloadx", gloadx_instr);

	// allocation
	registerOpcode("new", new_instr);
	registerOpcode("newx", newx_instr);
	registerOpcode("onew", onew_instr);

	// super constructor
	registerOpcode("callsuper", callsuper_instr);

	// invocation
	registerOpcode("call", call_instr);
	registerOpcode("gcall", gcall_instr);
	registerOpcode("callindex", callindex_instr);
	registerOpcode("callproperty", callproperty_instr);
	registerOpcode("callstatic", callstatic_instr);
	registerOpcode("chain", chain_instr);
	registerOpcode("ocall", ocall_instr);
	registerOpcode("callx", callx_instr);
	registerOpcode("gcallx", gcallx_instr);
	registerOpcode("callpropertyx", callpropertyx_instr);
	//registerOpcode("callstaticx", callstaticx_instr);
	registerOpcode("chainx", chainx_instr);

	// type operations
	registerOpcode("typeof", typeof_instr);
	registerOpcode("classof", classof_instr);
	registerOpcode("instanceof", instanceof_instr);
	registerOpcode("is", is_instr);
	registerOpcode("as", as_instr);
    
    // invocation via named arguments
    registerOpcode("namedcallx", namedcallx_instr);
    registerOpcode("namedocall", namedocall_instr);
    registerOpcode("namedcallproperty", namedcallproperty_instr);
    registerOpcode("namedcallindex", namedcallindex_instr);
    
    // errors
    registerOpcode("throw", throw_instr);
    
	// debug
	registerOpcode("log", log_instr);
}

H3oAssembler::~H3oAssembler() {}

void H3oAssembler::push(BlockNode *node)
{
	_stack.push(node);
}

void H3oAssembler::pop()
{
	_stack.pop();
}

void H3oAssembler::addSlot(LocalVar *var)
{
	getEnclosingFunc()->addSlot(var);
}

void H3oAssembler::addSlot(VM_Func *func)
{
	if (_stack.empty())
		return;

	_stack.top()->addSlot(func);
}

void H3oAssembler::addSlot(VM_Class *clss)
{
	if (_stack.empty())
		return;

	_stack.top()->addSlot(clss);
}

FuncNode *H3oAssembler::getEnclosingFunc()
{
    Ast *node = topNode();
    
    while(node)
    {
        if(FuncNode *func = dynamic_cast<FuncNode *>(node))
            return func;
        
        node = node->parent();
    }
    
    // default = main function (which is anonymous and generated implicitly in Hydro programming language)
    return _mainNode;
}

EventNode *H3oAssembler::getContextParent()
{
    return dynamic_cast<EventNode *>(topNode());
}

EventNode *H3oAssembler::getEventParent()
{
    return dynamic_cast<EventNode *>(topNode());
}

EventNode *H3oAssembler::getEnclosingEvent()
{
    Ast *node = topNode();
    
    while(node)
    {
        if(EventNode *event = dynamic_cast<EventNode *>(node))
            return event;
        
        node = node->parent();
    }
    
    // fail
    return nullptr;
}

ClassNode *H3oAssembler::getEnclosingClass()
{
    Ast *node = topNode();
    
    while(node)
    {
        if(ClassNode *clss = dynamic_cast<ClassNode *>(node))
            return clss;
        
        node = node->parent();
    }
    
    // fail
    return nullptr;
}

ClassNode *H3oAssembler::getOwnerClass()
{
    return topNode()->tryCast<ClassNode>();
}

void H3oAssembler::validatePresence(hydro_assembly_directive dir)
{
	BlockNode *node = _stack.top();

	if (dynamic_cast<FuncNode *>(node))
	{
		switch (dir)
		{
		case h3o_var_directive:
		case h3o_func_directive:
		case h3o_arg_directive:
		case h3o_class_directive:
		case h3o_event_directive:
		case h3o_action_directive:
			return; // valid
		default:
		{
			appendError("Directive is unexpected", currentToken());
			abort();
			break;
		}
		}
	}
	else if (dynamic_cast<ClassNode *>(node))
	{
		switch (dir)
		{
		case h3o_var_directive:
		case h3o_func_directive:
		case h3o_class_directive:
		case h3o_construct_directive:
		case h3o_property_directive:
		case h3o_method_directive:
		case h3o_event_directive:
		case h3o_action_directive:
			return; // valid
		default:
		{
			appendError("Directive is unexpected", currentToken());
			abort();
			break;
		}
		}
	}
}

void H3oAssembler::emit(H3oNode *node)
{
	_stack.top()->append(node);
}

void H3oAssembler::emit(int32_t i)
{
    emit(new IntNode{i});
}

void H3oAssembler::emit(hvm_isa_opcodes instr)
{
	emit(new InstrNode{instr});
}

void H3oAssembler::emit(hvm_isa_opcodes instr, int32_t opnd1)
{
	emit(new InstrNode{instr, opnd1});
}

void H3oAssembler::emit(hvm_isa_opcodes instr, int32_t opnd1, int32_t opnd2)
{
	emit(new InstrNode{instr, opnd1, opnd2});
}

void H3oAssembler::emit(hvm_isa_opcodes instr, int32_t opnd1, int32_t opnd2, int32_t opnd3)
{
	emit(new InstrNode{instr, opnd1, opnd2, opnd3});
}

uint32_t H3oAssembler::registerName(std::string name)
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

uint32_t H3oAssembler::registerConstant(hvalue constant)
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

Label *H3oAssembler::defineLabel(std::string label)
{
	for (Label *lab : _labels)
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
	Label *lab = new Label{label};
	lab->define();
	_labels.push_back(lab);
	return lab;
}

Label *H3oAssembler::getLabel(std::string label)
{
	for (Label *l : _labels)
		if (l->value() == label)
			return l;

	_labels.push_back(new Label{label});
	return _labels.back();
}

void H3oAssembler::parseConst()
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

void H3oAssembler::parseShortConst()
{
    validate(short_tkn);
    emit(push_instr, registerConstant((int16_t)std::stoi(tokenVal()))); // TODO string to short
}

void H3oAssembler::parseUshortConst()
{
    validate(ushort_tkn);
    emit(push_instr, registerConstant((uint16_t)std::stoi(tokenVal()))); // TODO string to ushort
}

void H3oAssembler::parseIntConst()
{
	validate(int_tkn);
	emit(push_instr, registerConstant(std::stoi(tokenVal())));
}

void H3oAssembler::parseUintConst()
{
	validate(uint_tkn);
	uint32_t i = (uint32_t)std::stoi(tokenVal()); // TODO string to uint
	emit(push_instr, registerConstant(i));
}

void H3oAssembler::parseLongConst()
{
    validate(long_tkn);
    emit(push_instr, registerConstant((int64_t)std::stol(tokenVal())));
}

void H3oAssembler::parseUlongConst()
{
    validate(ulong_tkn);
    emit(push_instr, registerConstant((uint64_t)std::stol(tokenVal())));
}

void H3oAssembler::parseStringConst()
{
	emit(push_instr, registerConstant(readString())); // TODO push string pointer
}

void H3oAssembler::parseFloatConst()
{
	validate(float_tkn);
	emit(push_instr, registerConstant(std::stof(tokenVal())));
}

void H3oAssembler::parseDoubleConst()
{
	validate(double_tkn);
	emit(push_instr, registerConstant(std::stod(tokenVal())));
}

void H3oAssembler::parseCharConst()
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

int32_t H3oAssembler::readInt()
{
	validate(int_tkn);
	return std::stoi(tokenVal());
}

std::string H3oAssembler::readString()
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

TypeSpecifier *H3oAssembler::parseTypeSpec()
{
	validate(":");
	advance(); // to expected type name

	TypeSpecifier *type;

	if (!match("*"))
	{
		type = nullptr;
		/*
             std::vector<std::string> cache;
             std::string qname; // qualified name
             
             while(tokenExists())
             {
             validate(id_tkn);
             qname += tokenVal();
             
             if(match("/"))
             {
             advance();
             qname += "/";
             continue;
             }
             else if(match("|"))
             {
             cache.push_back(qname);
             qname = "";
             continue;
             }
             else
             {
             cache.push_back(qname);
             break;
             }
             }
             
             std::string *all = new std::string[cache.size()];
             uint32_t i = 0;
             
             // copy
             for(std::string n : cache)
             all[++i] = nullptr;//cache[++i];
             */
	}
	else
		type = new NilTypeSpecifier{}; // any type

	return type;
}

uint16_t H3oAssembler::parseMods()
{
	uint16_t mod = 0;

	validate("<");

	do
	{
		advance(); // to expected identifier

		if (match("public"))
			mod |= mod_public; // global level access
		else if (match("internal"))
			mod |= mod_internal; // package level access
		else if (match("protected"))
			mod |= mod_protected; // sub-class level access
		else if (match("private"))
			mod |= mod_private; // class level access
		else if (match("restricted"))
			mod |= mod_restricted; // vm only access
		else if (match("abstract"))
			mod |= mod_abstract; // marks a method or type as abstract (prevents direct instantiation)
		else if (match("dynamic"))
			mod |= mod_dynamic; // enables dynamic members
		else if (match("final"))
			mod |= mod_final; // finalizes member
		else if (match("override"))
			mod |= mod_override;
		else if (match("native"))
			mod |= mod_native;
		else if (match("static")) // binds a definition to a type
			mod |= mod_static;
        else if (match("readonly"))
            mod |= mod_readonly;
        else if (match("writeonly"))
            mod |= mod_writeonly;
        else if (match("synchronized"))
            mod |= mod_synchronized;
		else
			abrupt();

		if (matchNext("|"))
		{
			advance(); // to |
			continue;
		}

		// end
		break;
	} while (true);

	advance(); // to expected >
	validate(">");

	return mod;
}

std::string H3oAssembler::readType()
{
	if (match(id_tkn))
	{
		return tokenVal();
	}
	else
	{
		validate(package_tkn);
		return tokenVal().substr(1); // omit @
	}
}

void H3oAssembler::parseInstr()
{
    hvm_isa_opcodes opcode = static_cast<hvm_isa_opcodes>(readOpcode());

	switch (opcode)
	{
	case pop_instr:	// pop top operand
	case dup_instr:	// duplicate top operand
	case swp_instr:	// swap top two operands
	case hlt_instr:	// halt
	case ret_instr:	// return top operand (pops the stack)
	case true_instr:	// true
	case false_instr:	// false
	case null_instr:	// null
	case undefined_instr:	// undefined
	case inf_instr:	// Inifinity
	case nan_instr:	// NaN
	case object_instr:   // object
	case json_instr:     // json
	case xml_instr:	// xml
	case list_instr:     // list
	case listpush:	 // list push
	case dict_instr:     // dict
	case dictinit_instr: // dict init
	case pos_instr:	// positive
	case neg_instr:	// negation
	case exp_instr:	// exponential
	case mul_instr:	// multiplcation
	case div_instr:	// division
	case mod_instr:	// modulo
	case add_instr:	// addition
	case sub_instr:	// subtraction
	case leq_instr:	// logical equality
	case liq_instr:	// logical inequality
	case seq_instr:	// strict equality
	case siq_instr:	// strict inequality
	case lnt_instr:	// logical NOT
	case gt_instr:	 // greater than
	case gte_instr:	// greater than or equal
	case lt_instr:	 // less than
	case lte_instr:	// less than or equal
	case is_instr:	 // is operator
	case as_instr:	 // as operator
	case bnt_instr:	// bitwise NOT
	case bnd_instr:	// bitwise AND
	case bor_instr:	// bitwise OR
	case bxr_instr:	// bitwise XOR
	case brs_instr:	// bitwise right shift
	case bls_instr:	// bitwise left shift
	case log_instr:	// log (debugger)
	case setindex_instr: // store index
	case getindex_instr: // load index
	case this_instr:     // load this object
    case throw_instr: // throw
	{
		emit(opcode);
		break;
	}
	case push_instr: // push constant
	{
		advance(); // to expected constant
		parseConst();
		break;
	}
	case store_instr:		// store local by index
	case load_instr:		// load local by index
	case setproperty_instr: // store field by index
	case getproperty_instr: // load field by index
	case gstore_instr:	// store global by index
	case gload_instr:		// load global by index
	{
		advance(); // to expected index
		emit(opcode, readInt());
		break;
	}
	case storex_instr:	 // store local by name
	case loadx_instr:		 // load local by name
	case setpropertyx_instr: // store field by name
	case getpropertyx_instr: // load field by name
	case gstorex_instr:	// store global by name
	case gloadx_instr:	 // load global by name
	case objectinit_instr:   // object init
    case inc_instr:
    case incproperty_instr:
    case incindex_instr:
    case dec_instr:
    case decproperty_instr:
    case decindex_instr:
	{
		advance(); // to expected name
		validate(id_tkn);
		emit(opcode, registerName(tokenVal()));
		break;
	}
	case call_instr:		 // call local by index
	case gcall_instr:		 // call global by index
	case callproperty_instr: // call property by index
	case chain_instr:		 // call property by index via method chaining
	{
		advance();
		int32_t index = readInt(); // initialize i
		advance();			   // to expected nargs
		int32_t nargs = readInt();
		emit(opcode, index, nargs);
		break;
	}
	case callindex_instr: // call index
	case ocall_instr:     // call top operand
	case callsuper_instr: // call super constructor
	{
		advance(); // to expected nargs
		emit(opcode, readInt());
		break;
	}
	case callx_instr:		  // call local by index
	case gcallx_instr:	  // call global by index
	case callpropertyx_instr: // call property by index
	case chainx_instr:	  // call property by index via method chaining
	case newx_instr:		  // allocate instance by local name
	{
		advance(); // to expected name
		validate(id_tkn);
		int32_t index = registerName(tokenVal());
		advance(); // to expected nargs
		int32_t nargs = readInt();
		emit(opcode, index, nargs);
		break;
	}
	case rload_instr: // load a class by name
	{
		std::string className;
		advance(); // to expected name
		if (match(package_tkn))
		{
			className = tokenVal().substr(1); // omit @
		}
		else
		{
			validate(id_tkn);
			className = tokenVal();
		}
		emit(opcode, registerName(className));
		break;
	}
	case jmp_instr: // branch
	case brt_instr: // branch if true
	case brf_instr: // branch if false
	case lnd_instr: // logical AND
	case lor_instr: // logical OR
	{
		advance(); // to expected identifier
		validate(id_tkn);
		Label *label = getLabel(tokenVal());
		emit(opcode);
		emit(new LabeledNode(label));
		break;
	}
	case new_instr: // allocate object instance (heap memory)
	{
		std::string name; // qualified name
		advance();		// to expected identifier or package
		if (match(id_tkn))
		{
			name = tokenVal();
		}
		else if (match(package_tkn))
		{
			name = tokenVal().substr(1);
		}
		else
		{
			//
			abrupt();
		}
		advance(); // to expected integer
		int32_t nargs = readInt();

		if (hclass c = _result->runtime->findClass(name))
		{
			emit(push_instr, registerConstant(c));
			emit(opcode, nargs);
		}
		else
		{
			uint32_t index = (uint32_t)_constPool.size();
			_constPool.push_back(undefined);
			emit(new NewNode{name, index, nargs});
		}
		break;
	}
	case onew_instr: // create new instance from top operand
	{
		advance(); // to expected integer
		int32_t nargs = readInt();
		emit(new_instr, nargs);
		break;
	}
	case jsoninit_instr: // json init
	{
		advance(); // to expected string
		emit(opcode, registerConstant(readString()));
		break;
	}
    case namedcallx_instr:
    case namedcallproperty_instr:
    {
        advance(); // to expected name
        validate(id_tkn);
        std::string name = tokenVal();
        advance(); // to expected int
        int32_t nargs = readInt();
        emit(opcode, registerName(name), nargs);
        while(nargs-- > 0)
        {
            advanceSkip(ln_tkn); // to expected arg name
            validate(id_tkn);
            std::string argName = tokenVal();
            emit((int32_t)registerName(argName));
        }
        break;
    }
    case namedocall_instr:
    case namedcallindex_instr:
    {
        advance(); // to expected int
        int32_t nargs = readInt();
        emit(opcode, nargs);
        while(nargs-- > 0)
        {
            advance(); // to expected arg name
            validate(id_tkn);
            std::string argName = tokenVal();
            emit((int32_t)registerConstant(argName));
        }
        break;
    }
	default:
	{
		// unrecognized instruction
		appendError("Encountered unrecognized opcode instruction '" + tokenVal() + "'.", currentToken());
		break;
	}
	}

	if (nextTokenExists())
	{
		advance(); // to expected linefeed
		validate(ln_tkn);
	}
}

void H3oAssembler::parseBody(BlockNode *node, bool isDecl)
{
	validate("{");
	push(node);
    
    if(isDecl)
        _blocks.push_back(node);
    
	while (!matchNext("}", ln_tkn) && nextTokenExists())
	{
		advanceSkip(ln_tkn); // to next token
		parseNext();
	}

	pop();
	advanceSkip(ln_tkn);
	validate("}");
    
}

void H3oAssembler::parseArg(FuncNode *node)
{
    validate(".arg");
    VM_Func *func = node->func();

    // variadic ?
    if (matchNext("$"))
    {
        advance(); // to $
        if (func->variadic)
            appendError("Encountered numerous variadic arguments.", currentToken());
        func->variadic = true;
    }
    else if (func->variadic)
    {
        // error
        appendError("Variadic flag must be marked on the last argument in the argument list.", currentToken());
        return;
    }
    
    advance();        // to expected name
    validate(id_tkn); // identifier
    std::string name = tokenVal();
    advance(); // to expected type specifier
    TypeSpecifier *type = parseTypeSpec();

    FuncArg *arg = new FuncArg{};
    arg->name = name;
    arg->type = type;

    InstrGroupNode *defaultVal = nullptr;

    if (matchNext("="))
    {
        advance(); // to =
        advance(); // to expected [
        defaultVal = parseInstrGroup();
    }
    
    node->appendArg(new FuncArgNode{arg, defaultVal});
}

InstrGroupNode *H3oAssembler::parseInstrGroup()
{
	validate("[");
	InstrGroupNode *group = new InstrGroupNode{new InstrGroup{}};
	push(group); // push group

	while (nextTokenExists() && !matchNext("]"))
	{
		advance();    // to expected instr
		skip(ln_tkn); // skip linefeeds
		parseInstr();
	}

	advance();    // to expected ]
	skip(ln_tkn); // skip linefeeds
	validate("]");
	pop(); // pop group
	return group;
}

void H3oAssembler::parseTryCatch()
{
    validate(".try");
    FuncNode *func = getEnclosingFunc();
    Label *start = new Label{""};
    Label *end = new Label{""};
    Label *jump = new Label{""};
    TryNode *node = new TryNode{func, start, end, jump};
    func->appendTry(node);
    advanceSkip(ln_tkn); // to expected block
    parseBody(node, false);
    start->define();
    end->define();
    jump->define();
    
    while (matchNext(".catch", ln_tkn))
    {
        advanceSkip(ln_tkn);
        validate(".catch");
        advance(); // to expected variable index
        int32_t slot = readInt();
        advance(); // to expected type specifier
        parseTypeSpec();
        CatchNode *ctch = new CatchNode{slot, nullptr, start, end, jump};
        node->append(ctch);
        advanceSkip(ln_tkn); // to expected block
        parseBody(ctch, false);
    }
}

void H3oAssembler::parseNext()
{
	skip(ln_tkn); // skip trailing linefeeds
	if (!nextTokenExists())
		return;

	std::string pkg, name, qname;
	TypeSpecifier *type;
	uint16_t mod = 0; // default modifier
    bool isRuntimeDefinition = false;
    
	if (match(".var"))
	{
		// variable
		// .var name @system/String
		// .var name
		advance(); // to expected name
		validate(id_tkn);
		name = tokenVal();
		advance(); // to expected type specifier
		type = parseTypeSpec();
		LocalVar *var = new LocalVar{};
		var->name = name;
		var->type = type;
		addSlot(var);
	}
	else if (match(".func"))
	{
		// function
		// .func name? (*) @system/String|[byte]|<int|double, int|double, bool> : <global>
		validatePresence(h3o_func_directive);
		std::string name = "";
		//int32_t index = _stack.top()->nslots();

		if (matchNext(id_tkn))
		{
			advance(); // to identifier
			name = tokenVal();
		}

		advance(); // to expected (
		validate("(");
		advance(); // to expected )
		validate(")");

		if (matchNext(package_tkn))
		{
			advance();			    // to
			pkg = tokenVal().substr(1); // omit @

			advance(); // to expected modifier
			mod = parseMods();

			// set full qualified name
			qname = pkg + "/" + name;
		}
		else
		{
			// set qualified name as function name
			qname = name;

			if (matchNext("<"))
			{
				advance(); // to mods
				mod = parseMods();
			}
		}

		advance(); // to expected return type
		type = parseTypeSpec();

		VM_Func *func = new VM_Func{};
		func->chunk = _result;
		func->name = qname; // might be anonymous
		func->modifier = mod;
		func->returnType = type;

        FuncNode *node = new FuncNode{func};
        
		if (!pkg.empty())
		{
			// define in runtime
            hvalue fn = _env->createAndBindIfNotExists(_runtime, func);
            emit(push_instr, registerConstant(fn));
		}
		else
		{
			// add local
			addSlot(func);
            //topNode()->appendFront(new InstrNode(push_instr, registerConstant(_env->toHvmObject(func))));
            topNode()->appendFront(new InstrNode(definex_instr, registerConstant(_env->toHvmObject(func))));
		}
        
        while(matchNext(".arg", ln_tkn))
        {
            advanceSkip(ln_tkn); // to .arg
            parseArg(node);
        }

		if (has_modifier(mod, mod_native) || has_modifier(mod, mod_abstract))
		{
			// block is not allowed
			if (matchNext("{", ln_tkn))
			{
                delete func; // dispose
                delete node; // dispose
				advanceSkip(ln_tkn); // to unexpected block
				abrupt();
			}
            
            emit(node);
		}
		else
		{
			advanceSkip(ln_tkn); // to expected block
			parseBody(node);
		}
	}
    else if(match(".try"))
    {
        // try
        parseTryCatch();
    }
	else if (match(".action"))
	{
		validatePresence(h3o_action_directive);
        
        advance(); // to expected name
        validate(id_tkn);
        name = tokenVal();
        
        advance(); // to expected [
        validate("[");
        
        advance(); // to expected type
        validate(package_tkn);
        std::string context = tokenVal().substr(1);
        advance(); // to expected ]
        validate("]");
        
        if (matchNext(package_tkn))
        {
            advance();                // to
            pkg = tokenVal().substr(1); // omit @

            advance(); // to expected modifier
            mod = parseMods();

            // set full qualified name
            qname = pkg + "/" + name;
        }
        else
        {
            // set qualified name as function name
            qname = name;

            if (matchNext("<"))
            {
                advance(); // to mods
                mod = parseMods();
            }
        }
        
        VM_Action *act = new VM_Action{};
        act->chunk = _result;
        act->name = qname; // might be anonymous
        act->modifier = mod;
        act->returnType = new NilTypeSpecifier{};

        ActionNode *node = new ActionNode{act, context};
        
        if (!pkg.empty())
        {
            // define in runtime
            // TODO use a node to delay this process
            hvalue fn = _env->createAndBindIfNotExists(_runtime, act);
            emit(push_instr, registerConstant(fn));
        }
        else
        {
            // add local
            addSlot(act);
            //topNode()->appendFront(new InstrNode(push_instr, registerConstant(_env->toHvmObject(act))));
            topNode()->appendFront(new InstrNode(definex_instr, registerConstant(_env->toHvmObject(act))));
        }
        
        if (has_modifier(mod, mod_native) || has_modifier(mod, mod_abstract))
        {
            // block is not allowed
            if (matchNext("{", ln_tkn))
            {
                delete act; // dispose
                delete node; // dispose
                advanceSkip(ln_tkn); // to unexpected block
                abrupt();
            }
            
            emit(node);
        }
        else
        {
            advanceSkip(ln_tkn); // to expected block
            parseBody(node);
        }
	}
    else if (match(".event"))
    {
        // event
        advance(); // to expected name
        validate(id_tkn);
        name = tokenVal();
        std::string superEvent;
        
        if(matchNext(package_tkn))
        {
            advance(); // to package
            pkg = tokenVal().substr(1);
            
            // set full qualified name
            qname = pkg + "/" + name;
            isRuntimeDefinition = true;
            
            advance(); // to expected modifier
            mod = parseMods();
        }
        else
        {
            // set qualified name as class name
            qname = name;
            
            if (matchNext("<"))
            {
                advance(); // to expected modifier
                mod = parseMods();
            }
        }
        
        if(matchNext("extends"))
        {
            advance(); // to expected extends
            validate("extends");
            advance(); // to expected full type name or slot index
            if (match(int_tkn))
            {
                // is local or upvalue
            }
            else
            {
                superEvent = readType();
            }
        }
        
        VM_Event *event = nullptr;
        //event = _env->loadReservedEvent(qname);
        
        if(event)
        {
            if(!is_native(mod))
                appendError("Reserved event '" + qname + "' was not declared as native.");
        }
        else
        {
            // create event data
            event = new VM_Event{};
        }
        
        event->modifier = mod;
        event->name = qname;
        
        // try get base event (first attempt)
        hevent super = _runtime->findEvent(superEvent);
        
        if(super)
        {
            // success!
            event->base = super->vevent();
        }
        
        EventNode *node = new EventNode{event};
        advanceSkip(ln_tkn); // to expected block
        parseBody(node);
        
        if(!pkg.empty())
        {
            //_env->createAndBindIfNotExists(_runtime, event);
        }
        else
        {
            appendError("Local events are not yet supported.");
        }
    }
    else if (match(".etype"))
    {
        advance(); // to expected name
        validate(id_tkn);
        name = tokenVal();
        EventNode *event = getEventParent();
        if(!event)
            appendError("Etypes must be defined directly in the block of an Event declaration.");
        VM_Etype *etype = new VM_Etype{};
        etype->owner = event->vevent();
        etype->name = name;
        etype->modifier = mod_public | mod_final;
        event->appendEtype(etype);
    }
    else if (match(".param"))
    {
        advance(); // to expected name
        validate(id_tkn);
        name = tokenVal();
        advance(); // to expected type
        TypeSpecifier* type = parseTypeSpec();
        EventNode *context = getContextParent();
        
        if(!context)
            appendError("Context parameters must be defined directly in the block of an Event declaration.");
        
        if(matchNext("<"))
        {
            advance(); // to optional modifiers
            mod = parseMods();
        }
        
        EventParam *param = new EventParam{};
        param->modifier = mod;
        param->type = type;
        param->name = name;
        InstrGroupNode *defaultValue = nullptr;
        
        if (matchNext("="))
        {
            advance(); // to expected default value
            advance(); // to expected instr group
            defaultValue = parseInstrGroup();
        }
        
        context->appendParam(new ParamNode{param, defaultValue});
    }
	else if (match(".class"))
	{
		validatePresence(h3o_class_directive);

		std::string superClass;
        
		// class
		advance(); // to expected name
		validate(id_tkn);
		name = tokenVal();

		if (matchNext(package_tkn))
		{
			advance(); // to package
			pkg = tokenVal().substr(1);

			// set full qualified name
			qname = pkg + "/" + name;
			isRuntimeDefinition = true;

			advance(); // to expected modifier
			mod = parseMods();
		}
		else
		{
			// set qualified name as class name
            qname = name;
            
			if (matchNext("<"))
			{
				advance(); // to expected modifier
				mod = parseMods();
			}
		}
        
        if(matchNext("extends"))
        {
            advance(); // to expected extends
            validate("extends");
            advance(); // to expected full type name or slot index
            if (match(int_tkn))
            {
                // is local or upvalue
            }
            else
            {
                superClass = readType();
            }
        }
        
        VM_Class *clss;
        clss = _env->core()->loadReserved(qname);
        if(clss)
        {
            // validate reserved class
            if(!is_native(mod))
                appendError("Reserved class '" + qname + "' was not a declared as native."); // implementation error
        }
        else
        {
            // create class data
            clss = new VM_Class{};
        }
        
		clss->name = qname;
		clss->modifier = mod;
		clss->chunk = _result;
        
        // try get base class (first attempt)
        hclass super = _runtime->findClass(superClass);
        
        if(super)
        {
            // success!
            clss->base = super->vclass();
        }
        
		advanceSkip(ln_tkn); // to expected block
		ClassNode *node = new ClassNode{_result, clss, superClass};
		parseBody(node);

		if (!isRuntimeDefinition)
		{
			// add local
			addSlot(clss);
			//topNode()->appendFront(new InstrNode(push_instr, registerConstant(_env->toHvmObject(clss))));
            topNode()->appendFront(new InstrNode(definex_instr, registerConstant(_env->toHvmObject(clss))));
		}
	}
	else if (match(".construct"))
	{
		validatePresence(h3o_construct_directive);
		lex_token tkn = currentToken();
		advance(); // to expected (
		validate("(");
		advance(); // to expected )
		validate(")");

		advance(); // to expected modifiers
		mod = parseMods();

        ClassNode *owner = getOwnerClass();

		if (owner)
		{
            if(owner->getConstructor())
            {
                appendError("Constructor cannot be redefined.", tkn);
            }
            else
            {
                VM_Constructor *construct = new VM_Constructor{};
                construct->chunk = _result;
                construct->owner = owner->vclass();
                construct->modifier = mod;
                construct->name = ""; // not applicable
                construct->returnType = new NilTypeSpecifier{};
                construct->package = ""; // not applicable
                ConstructorNode *node = new ConstructorNode{construct};
                
                while(matchNext(".arg", ln_tkn))
                {
                    advanceSkip(ln_tkn); // to .arg
                    parseArg(node);
                }
                
                if (has_modifier(mod, mod_native))
                {
                    // block is not allowed
                    if (matchNext("{"))
                    {
                        delete construct; // dispose
                        delete node; // dispose
                        advance(); // to unexpected block
                        abrupt();
                    }
                }
                else
                {
                    advanceSkip(ln_tkn); // to expected block
                    parseBody(node);
                }
                
                construct->owner->construct = construct;
                owner->setConstructor(node);
            }
		}
		else
		{
			// not permitted
			appendError("Constructor directive is only permitted within a class directive block.", tkn);
		}
	}
	else if (match(".property"))
	{
		validatePresence(h3o_property_directive);

		// property
		// .property name @system\String|[@system:Int32] : <public>
        Token *tkn = currentToken();
		advance(); // to expected identifier
		validate(id_tkn);
		name = tokenVal();
		advance(); // to expected type specifier
		type = parseTypeSpec();
		advance(); // to expected modifier
		mod = parseMods();
        
        ClassNode *owner = getOwnerClass();
        
        if(owner)
        {
            VM_Property *prop = new VM_Property{};
            prop->modifier = mod;
            prop->name = name;
            prop->type = type;
            prop->owner = owner->vclass();
            PropertyNode *node = new PropertyNode{prop};
            owner->addProperty(node);
            
            if (matchNext("{", ln_tkn))
            {
                advanceSkip(ln_tkn); // to expected block
                parseBody(node);
            }
            else if (matchNext("="))
            {
                advance(); // to =
                advance(); // to expected [
                InstrGroupNode *grp = parseInstrGroup();
                node->initValue = grp;
            }
        }
        else
        {
            // not permitted
            appendError("Property directive is only permitted within a class directive block.", tkn);
        }
	}
	else if (match(".getter"))
	{
		validatePresence(h3o_getter_directive);

		// getter
        advance(); // to expected (
        validate("(");
        bool autoProp = false;
        
        if(matchNext("auto"))
        {
            advance(); // to auto
            autoProp = true;
        }
        
        advance(); // to expected )
        validate(")");
        
        
        while(matchNext(".arg", ln_tkn))
        {
            advanceSkip(ln_tkn); // to .arg
            appendError("Arguments are not permitted on getter directives.", currentToken());
        }

        PropertyNode *owner = topNode()->tryCast<PropertyNode>();

        if (owner)
        {
            VM_Getter *vgetter = new VM_Getter{};
            vgetter->automatic = autoProp;
            vgetter->chunk = _result;
            GetterNode *node = new GetterNode{owner, vgetter};
            
            if(matchNext("{", ln_tkn))
            {
                advanceSkip(ln_tkn);
                
                if(autoProp)
                    abrupt(); // invalid
                
                parseBody(node, false);
                owner->append(node);
            }
            else if(autoProp)
            {
                owner->append(node);
            }
        }
        else
        {
            // invalid
            appendError("Getter directive defined outside of property scope.");
        }
	}
	else if (match(".setter"))
	{
		validatePresence(h3o_setter_directive);

		// setter
        advance(); // to expected (
        validate("(");
        bool autoProp = false;
        
        if(matchNext("auto"))
        {
            advance(); // to auto
            autoProp = true;
        }
        
        advance(); // to expected )
        validate(")");
        
        PropertyNode *owner = topNode()->tryCast<PropertyNode>();

        if (owner)
        {
            VM_Setter *vsetter = new VM_Setter{};
            vsetter->automatic = autoProp;
            vsetter->chunk = _result;
            SetterNode *node = new SetterNode{owner, vsetter};

            while(matchNext(".arg", ln_tkn))
            {
                advanceSkip(ln_tkn); // to .arg
                parseArg(node);
            }
            
            if(matchNext("{", ln_tkn))
            {
                advanceSkip(ln_tkn);
                
                if(autoProp)
                    abrupt(); // invalid
                
                
                parseBody(node, false);
                owner->append(node);
            }
            else if(autoProp)
            {
                owner->append(node);
            }
        }
        else
        {
            // invalid
            appendError("Getter directive defined outside of property scope.");
        }
	}
	else if (match(".method"))
	{
		// method
		// .method name? (*) @system/String|[byte]|<int|double, int|double, bool> : <global>
		validatePresence(h3o_method_directive);
		std::string name = "";
		lex_token tkn = currentToken();

		if (matchNext(id_tkn))
		{
			advance(); // to identifier
			name = tokenVal();
		}

		advance(); // to expected (
		validate("(");
		advance(); // to expected )
		validate(")");

		advance(); // to expected modifier
		mod = parseMods();

		advance(); // to expected return type
		type = parseTypeSpec();

		ClassNode *owner = getOwnerClass();

		if (owner)
		{
			VM_Method *method = new VM_Method{};
			method->owner = owner->vclass();
			method->chunk = _result;
			method->name = name; // might be anonymous
			method->modifier = mod;
			method->returnType = type;
            
            MethodNode *node = new MethodNode{method};
            
            while(matchNext(".arg", ln_tkn))
            {
                advanceSkip(ln_tkn); // to .arg
                parseArg(node);
            }

			if (has_modifier(mod, mod_native) || has_modifier(mod, mod_abstract))
			{
				// block is not allowed
				if (matchNext("{"))
				{
                    delete method; // dispose
                    delete node; // dispose
					advance(); // to unexpected block
					abrupt();
				}
                
                owner->addMethod(node);
			}
			else
			{
				advanceSkip(ln_tkn); // to expected block
				owner->addMethod(node);
				parseBody(node);
			}
		}
		else
		{
			// not permitted
			appendError("Method directive is only permitted within a class directive block.", tkn);
		}
	}
	else if (match(".event"))
	{
		validatePresence(h3o_event_directive);

		// event
	}
	else if (match(".etype"))
	{

		// etype
	}
	else if (match(id_tkn))
	{
		if (matchNext(":"))
		{
			if (Label *label = defineLabel(tokenVal()))
				emit(label); // emit label
			else
				appendError("The label '" + tokenVal() + "' is already defined.", currentToken());

			advance(); // to :
		}
		else
		{
			// instruction
			parseInstr();
			return;
		}
    }
	else
		abrupt();

	if (nextTokenExists())
	{
		advance(); // to expected linefeed
		validate(ln_tkn);
	}
}

void H3oAssembler::build()
{
    std::list<ClassNode *> visited;
    
	// number of names
	uint32_t nnames = (uint32_t)_names.size();

	// number of constants
	uint32_t nconst = (uint32_t)_constPool.size();

	// index
	uint32_t i;

	// copy names
	_result->names = new std::string[nnames];
	assert(_result->names);
	for (i = 0; i < _names.size(); i++)
		_result->names[i] = _names[i];

	// copy constants
	_result->constPool = new hvalue[nconst];
	assert(_result->constPool);
	i = 0;
	for (hvalue c : _constPool)
		_result->constPool[i++] = c;

	while (!_stack.empty())
	{
		delete _stack.top();
		_stack.pop();
	}

	// resolve forward references
	for (Label *label : _labels)
	{
		if (!label->isDefined())
		{
			// error
			appendError("The label '" + label->value() + "' was never defined.");
		}
	}
    
    // try resolve base types (final attempt)
    for (BlockNode *type : _blocks)
    {
        if(ClassNode *clss = dynamic_cast<ClassNode *>(type))
        {
            if(clss->vclass()->base || clss->superClassName().empty())
                continue; // skip
            
            for (BlockNode *node : _blocks)
            {
                if(clss->vclass()->base || node == type)
                    continue; // skip
                
                if(ClassNode *base = dynamic_cast<ClassNode *>(node))
                {
                    if(clss->superClassName() == base->vclass()->name)
                    {
                        // found
                        clss->vclass()->base = base->vclass();
                        break;
                    }
                }
            }
            
            if(!clss->vclass()->base)
                throw std::runtime_error{"Failed to bind base type '" + get_qualified_name(clss->superClassName()) + "' to type '" + get_qualified_name(clss->vclass()->name) + "'."};
        }
        else if(EventNode *evnt = dynamic_cast<EventNode *>(type))
        {
            if(evnt->vevent()->base || evnt->superEventName().empty())
                continue; // skip
            
            for (BlockNode *node : _blocks)
            {
                if(evnt->vevent()->base || node == type)
                    continue; // skip
                
                if(EventNode *base = dynamic_cast<EventNode *>(node))
                {
                    if(evnt->superEventName() == base->vevent()->name)
                    {
                        // found
                        evnt->vevent()->base = base->vevent();
                        break;
                    }
                }
            }
        }
    }

	// build symbols
	for (BlockNode *type : _blocks)
	{
        if(ClassNode *clss = dynamic_cast<ClassNode *>(type))
        {
            // build class
            buildClass(clss, visited);
        }
        else if(EventNode *evnt = dynamic_cast<EventNode *>(type))
        {
            // build event
            evnt->build(_result);
            if(!get_package_name(evnt->vevent()->name).empty())
                _env->createAndBindIfNotExists(_runtime, evnt->vevent());
        }
        else
        {
            // emit some blocked directive
            type->build(_result);
        }
	}
    
    // build actions
    for (BlockNode *type : _blocks)
    {
        if(ActionNode *act = dynamic_cast<ActionNode *>(type))
        {
            auto vact = act->vact();
            
            if(vact->context)
                continue; // skip
            
            hobject result = _result->runtime->find(act->contextName());
            
            if(hevent cxt = result.tryCast<HEventContext>())
            {
                // success!
                vact->context = cxt->vevent();
            }
            else
            {
                // error
                throw std::runtime_error{"Failed to locate context '" + act->contextName() + "'."};
            }
        }
    }
    
    // dipose
    for (BlockNode *node : _blocks)
        delete node;
    
    _blocks.clear();

	// build main program instructions
	_mainNode->build(_result);

	// set main function
	_result->main = _mainFunc;
	_result->size = nconst;
	_names.clear();
	_constPool.clear();
	_labels.clear();
	_types.clear();
	_blocks.clear();

	// clean-up
	delete _mainNode;
	_mainNode = nullptr;
}

void H3oAssembler::onParseInit()
{
	// reset
	_names.clear();
	_constPool.clear();
	_types.clear();
	_blocks.clear();

	// init chunk
	_result = new Chunk{};
	_result->runtime = _runtime;

	// create main function
	_mainFunc = new VM_Func{};
	_mainFunc->chunk = _result;
	_mainFunc->package = "";
	_mainFunc->name = "";					   // anonymous
	_mainFunc->args = nullptr;				   // not applicable
	_mainFunc->locals = nullptr;				   // will be already defined in "scopeToUse"
	_mainFunc->modifier = mod_restricted; // vm only code
	_mainFunc->returnType = new NilTypeSpecifier{};
	_mainFunc->variadic = false; // also not applicable
    
	// init main node
	_mainNode = new FuncNode{_mainFunc};
	_stack.push(_mainNode);
}

ClassNode *H3oAssembler::locateSuperClassNode(ClassNode *classNode)
{
    for(BlockNode *type : _blocks)
        if(ClassNode *clss = dynamic_cast<ClassNode *>(type))
            if(clss->vclass() == classNode->vclass()->base)
                return clss;
    
    // fail
    return nullptr;
}

void H3oAssembler::buildClass(ClassNode *classNode, std::list<ClassNode *> &visited)
{
    ClassNode *superClass = locateSuperClassNode(classNode);
    
    // build super class first (if exists)
    if(superClass)
        buildClass(superClass, visited);
    
    for(ClassNode *v : visited)
        if(v == classNode)
            return; // skip (already handled)
    
    VM_Class *vclass = classNode->vclass();
    visited.push_back(classNode);

    if(_runtime->find(vclass->name))
    {
        return; // already exists
    }
    
    
    // build
    classNode->build(_result);
    
    if(_env->core()->resolveAndRemoveReservedClass(vclass))
    {
        // skip
        return;
    }
    
    // late binding
    _env->createAndBindIfNotExists(_runtime, vclass);
}

void H3oAssembler::onParseComplete()
{
	pop();
	build();
    _result->bytes.freeUnusedMemory();
}

} // namespace hydro
