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

#include "VM.hpp"
#include "Runtime.hpp"
#include "HvmEnv.hpp"
#include "../system/HAction.hpp"
#include "../system/HFunction.hpp"
#include "../utility/classhelper.hpp"

namespace hydro
{

VM::VM(HvmEnv *env, MemorySpace *globals) : _env{env}, _globals{globals}, _activeThreads{0} {}

VM::~VM() {}

std::string VM::describe(hvalue val)
{
    return _env->util()->describe(val);
}

void VM::opush(CallFrame *frame, hvalue opnd)
{
    frame->operands.push_back(opnd);
}

hvalue VM::opop(CallFrame *frame)
{
	if (frame->operands.empty())
        throw std::runtime_error{"Stack is empty."};
    
    hvalue top = frame->operands.back();
    frame->operands.pop_back();
    return top;
}

hvalue VM::opeek(CallFrame *frame)
{
	if (frame->operands.empty())
        throw std::runtime_error{"Stack is empty."};
    
    return frame->operands.back();
}

CallFrame *VM::currentFrame(HvmContext *cxt)
{
	return cxt->currentFrame;
}

MemorySpace *VM::findVar(std::string name, MemorySpace *scope)
{
	while (scope)
	{
		if (scope->exists(name))
			return scope;

		scope = scope->parent();
	}

	// fail
	return nullptr;
}

MemorySpace *VM::findVarWithDynamicScaope(std::string name, CallFrame *frame)
{
	while (frame && frame->dynamic)
	{
		if (frame->locals->exists(name))
			return frame->locals;

		frame = frame->parentFrame;
	}

	// fail
	return nullptr;
}

bool VM::cpu(HvmContext *cxt, Chunk *chunk, CallFrame *currentFrame, hvalue &result)
{
	bool retVal = false;
	uint32_t addr;											 // any address
	uint32_t startAddr = currentFrame->startAddr, endAddr = currentFrame->endAddr; // start and end
	bytearray &bin = chunk->bytes;
	MemorySpace *scopeWithVar;
	uint8_t instr;
	hvalue a, b, c;	// operands
	int32_t i, j, index; // any int
	HObject *obj;
	std::string name;
	uint32_t &ip = currentFrame->ip;
    uint32_t &ipActive = currentFrame->ipActive;
    
	// init address
	ip = startAddr;

	// push current frame
	cxt->currentFrame = currentFrame;

	// emulate
	while (!currentFrame->halt && ip < endAddr && ip < bin.size())
	{
        
        if(cxt->errorPending)
        {
            // caught error
            cxt->errorPending = false;
            ip = cxt->errorAddr;
            continue;
        }
        
		instr = bin.get(ip); // read byte
        ipActive = ip;
        
        #ifdef HVM_TRACE
		// trace instruction
        if(currentFrame->trace)
            dissasemble(instr, ip, chunk);
        #endif
        
        try {

		// instruction dispatcher
		switch (instr)
		{
		case store_instr:
		{
			i = bin.getInt(ip);
			a = opeek(currentFrame); // value
			currentFrame->locals->slots[i]->content = a;
			break;
		}
		case load_instr:
		{
			i = bin.getInt(ip);
			opush(currentFrame, currentFrame->locals->slots[i]->content);
			break;
		}
		case gstore_instr:
		{
			a = opeek(currentFrame);	// value
			i = bin.getInt(ip); // slot
			//_globals->slots[i]->content = a;
			break;
		}
		case gload_instr:
		{
			i = bin.getInt(ip);
			//opush(currentFrame, _globals->slots[i]->content);
			break;
		}
		case define_instr:
		{
			a = opeek(currentFrame);
			i = bin.getInt(ip); // slot
			//std::cout << i << " : " << currentFrame->locals->size() << std::endl;
			//currentFrame->locals->slots[i]->content = a;
			break;
		}
		case true_instr:
		{
			// push true
			opush(currentFrame, true);
			break;
		}
		case false_instr:
		{
			// push false
			opush(currentFrame, false);
			break;
		}
		case null_instr:
		{
			// push null
			opush(currentFrame, nullptr);
			break;
		}
		case undefined_instr:
		{
			// push undefined
			opush(currentFrame, undefined);
			break;
		}
		case inf_instr:
		{
			// push positive Infinity
			opush(currentFrame, std::numeric_limits<double>::infinity());
			break;
		}
		case nan_instr:
		{
			// push NaN
			opush(currentFrame, std::nan(""));
			break;
		}
		case rload_instr:
		{
			// load definition 
			name = chunk->names[bin.getInt(ip)]; // qualified class name
			hobject def = chunk->runtime->find(name); // find definition
            
			if (def)
                opush(currentFrame, hvalue{def});
			else
				opush(currentFrame, undefined);
			break;
		}
		case this_instr:
		{
			opush(currentFrame, currentFrame->thisObject);
			break;
		}
		case object_instr:
		{
			// object
			opush(currentFrame, hvalue{_env->make()->object()});
			break;
		}
		case objectinit_instr:
		{
			name = chunk->names[bin.getInt(ip)];
			b = opop(currentFrame);  // value
			a = opeek(currentFrame); // object
			if (a.type() == typeid(HObject *))
			{
				HObject *obj = a;
				// todo test system::Object
				if (!obj)
                {
					runtimeError(cxt, "Cannot initialize property '" + name + "' on null.");
                }

				obj->_cxt->setProperty(_env, this, currentFrame->callee, cxt, name, b);
			}
			else
            {
				runtimeError(cxt, "Expecting operand to be a dictionary.");
            }
			break;
		}
		case json_instr:
		{
			opush(currentFrame, _env->make()->json());
			break;
		}
		case jsoninit_instr:
		{
			i = bin.getInt(ip);     // constant pool index
			c = opop(currentFrame);		 // value
			b = opeek(currentFrame);		 // json instance
			a = chunk->constPool[i]; // string constant
			hjson json = b;		 // cast
			json->_cxt->setIndex(_env, this, currentFrame->callee, cxt, a, c);
			break;
		}
		case list_instr:
		{
			// array
			opush(currentFrame, _env->make()->list());
			break;
		}
		case listpush:
		{
			// array list push
			b = opop(currentFrame);  // element
			a = opeek(currentFrame); // list
			obj = a;
			if (HList *list = dynamic_cast<HList *>(obj))
			{
				// push element
				list->push(b);
			}
			break;
		}
		case dict_instr:
		{
			// dictionary
			opush(currentFrame, _env->make()->dictionary());
			break;
		}
		case dictinit_instr:
		{
			c = opop(currentFrame);  // value
			b = opop(currentFrame);  // key
			a = opeek(currentFrame); // dictionary
			hdictionary dict = a;
			dict->_cxt->setIndex(_env, this, currentFrame->callee, cxt, b, c);
			break;
		}
		case tup_instr:
		{
			// tuple
			//i = bin.getInt(ip); // length of tuplele{i};
			//opush(currentFrame, new Tuple{(uint32_t)i});
			break;
		};
		case xml_instr:
		{
			// xml

			break;
		}
		case rgx_instr:
		{
			// regular expression

			break;
		}
		case typeof_instr:
		{
			a = opop(currentFrame); // subject
			if (is_undefined(a))
				opush(currentFrame, "undefined");
			else if (is_null(a) || is_object(a))
				opush(currentFrame, "object");
			else if (is_byte(a))
				opush(currentFrame, "byte");
			/*
                    a = opop(); // subject
                    a = wrap(a);
                    if (is_undefined(a))
                        opush(undefined);
                    else if (is_null(a))
                        opush(nullptr);
                    else if (a.type() == typeid(Object *))
                    {
                        Class *c = object_typeof(a);
                        if (c != nullptr)
                            opush(c);
                        else
                            opush(undefined);
                    }
                    else
                        opush(undefined);
                    */
			break;
		}
		case is_instr:
		{
			b = opop(currentFrame); // rhs
			a = opop(currentFrame); // lhs

			if (is_object(a) && is_object(b))
			{
				obj = a;
				HObject *type = b;
				opush(currentFrame, _env->typing()->check(type, obj));
				break;
			}

			// fail
			opush(currentFrame, false);
			break;
		}
		case as_instr:
		{
			b = opop(currentFrame); // rhs
			a = opop(currentFrame); // lhs
			//a = wrap(a);
			// TODO cast as
			opush(currentFrame, b);
			//log("Warning! Cast 'as' is not yet supported.");
			break;
		}
		case pop_instr:
		{
			opop(currentFrame);
			break;
		}
		case dup_instr:
		{
			a = opop(currentFrame);
			opush(currentFrame, a);
			opush(currentFrame, a); // duplicate
			break;
		}
		case swp_instr:
		{
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, b);
			opush(currentFrame, a); // such that a is on the top of the stack
			break;
		}
		case jmp_instr:
		{
			ip = bin.getInt(ip); // jump
			break;
		}
		case brt_instr:
		{
			addr = bin.getInt(ip);
			a = opop(currentFrame);
			if (a)
				ip = addr; // jump if true
			break;
		}
		case brf_instr:
		{
			addr = bin.getInt(ip);
			a = opop(currentFrame);
			if (!a)
				ip = addr; // jump if false
			break;
		}
		case push_instr:
		{
			i = bin.getInt(ip);
			a = chunk->constPool[i];
            
            if (is_const_function(a))
            {
                // closure is a function
                opush(currentFrame, _env->createFunction((const FuncData *)a));
            }
            else if (is_const_class(a))
            {
                // closure is a class
                opush(currentFrame, _env->makeClass((const VM_Class *)a));
            }
			else
			{
				// push constant
				opush(currentFrame, a);
			}
			break;
		};
		case new_instr:
		{
			j = bin.getInt(ip); // nargs
			a = opop(currentFrame);

			// read args
			std::list<hvalue> args{};
			while (j-- > 0)
				args.push_front(opop(currentFrame));

			if (hclass type = a.tryCast<HClass>())
			{
                HConstructor *construct = type->constructor();
                
                if(construct)
                {
                    // TODO verify access
                    if(!false)
                    {
                        // illegal access
                    }
                    
                    // try alloc instance
                    hvalue instance = type->alloc();
                    
                    // ensure instance exists
                    if(!instance)
                        instance = new HObject{_env, type};
                    
                    // call constructor
                    call(cxt, construct, args, b, instance);

                    if(cxt->errorPending)
                        continue;
                    
                    opush(currentFrame, instance);
                }
                else
                {
                    // class is not constructible
                    runtimeError(cxt, "Type '" + get_qualified_name(type->assemblyName()) + "' is not constructible.");
                }
			}
			else
			{
				// not a class
                runtimeError(cxt, "Cannot instantiate " + describe(a) + ".");
			}
			break;
		}
		case callsuper_instr:
		{
			j = bin.getInt(ip); // nargs

			// read args
			std::list<hvalue> args{};
			while (j-- > 0)
				args.push_front(opop(currentFrame));
            
            // check that current frame is constructor call
            if(HConstructor *constructor = dynamic_cast<HConstructor *>(currentFrame->callee))
            {
                hclass superClass = constructor->ownerClass()->superClass();
                if(!superClass)
                {
                    runtimeError(cxt, "Call to super constructor on non-constructible class.");
                }
                hconstructor construct = superClass->constructor();
                if(!construct)
                {
                    runtimeError(cxt, "Call to super constructor on non-constructible class.");
                }
                
                // get instance
                hvalue instance = currentFrame->thisObject;
                
                // call super constructor
                call(cxt, construct, args, b, instance);
            }
            else
            {
                // illegal!
                runtimeError(cxt, "Call to super constructor outside of a constructor context.");
            }
			break;
		}
		case neg_instr:
		{
			// negation
			opush(currentFrame, -opop(currentFrame));
			break;
		}
		case mul_instr:
		{
			// multiplication
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a * b);
			break;
		}
		case div_instr:
		{
			// division
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a / b);
			break;
		}
		case add_instr:
		{
			// addition
			b = opop(currentFrame);
			a = opop(currentFrame);
			if (is_string(a) || is_string(b))
			{
				// string concatenation
				std::string stringA = _env->cast()->string(a);
				std::string stringB = _env->cast()->string(b);
				std::string s = stringA + stringB;
                // TODO the following code fixes a major bug
				//c = _env->makeString(s);
                c = s;
				opush(currentFrame, c);
			}
			else
			{
				// add
				opush(currentFrame, a + b);
			}
			break;
		}
		case sub_instr:
		{
			// subtraction
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a - b);
			break;
		}
		case exp_instr:
		{
			// exponetial
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a.exp(b));
			break;
		}
		case mod_instr:
		{
			// exponetial
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a % b);
			break;
		}
		case pos_instr:
		{
			// positive
			opush(currentFrame, +opop(currentFrame));
			break;
		}
		case lnt_instr:
		{
			// logical NOT
			opush(currentFrame, !opop(currentFrame));
			break;
		}
		case lnd_instr:
		{
			// logical AND
			addr = bin.getInt(ip);
			b = opeek(currentFrame);
			if (!b)
			{
				// jump if true (without popping stack)
				ip = addr;
			}
			else
			{
				// false
				opop(currentFrame);
			}
			break;
		}
		case lor_instr:
		{
			// logical OR
			addr = bin.getInt(ip);
			b = opeek(currentFrame);
			if (b)
			{
				// jump if true (without popping stack)
				ip = addr;
			}
			else
			{
				// false
				opop(currentFrame);
			}
			break;
		}
		case leq_instr:
		{
			// equality
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a == b);
			break;
		}
		case liq_instr:
		{
			// inequality
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a != b);
			break;
		}
		case seq_instr:
		{
			// strict equality
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a == b);
			break;
		}
		case siq_instr:
		{
			// strict inequality
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a != b);
			break;
		}
		case gt_instr:
		{
			// greater than
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a > b);
			break;
		}
		case gte_instr:
		{
			// greater than or equal
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a >= b);
			break;
		}
		case lt_instr:
		{
			// less than
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a < b);
			break;
		}
		case lte_instr:
		{
			// less than or equal
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a <= b);
			break;
		}
        case inc_instr:
        {
            // call local by name
            index = bin.getInt(ip); // name index
            name = chunk->names[index];
            scopeWithVar = nullptr;
            
            // resolve local using dynamic scope
            if (currentFrame->dynamic)
                scopeWithVar = findVarWithDynamicScaope(name, currentFrame);

            // resolve local using static scope
            if (!scopeWithVar)
                scopeWithVar = findVar(name, currentFrame->locals);

            // if does not exist
            if (!scopeWithVar)
            {
                runtimeError(cxt, "Attempted access of undefined local symbol '" + name + "'.");
            }

            // get callable
            scopeWithVar->get(name, a);
            b = 1;
            scopeWithVar->set(name, a + b);
            scopeWithVar->get(name, c);
            opush(currentFrame, c);
            break;
        }
        case dec_instr:
        {
            // call local by name
            index = bin.getInt(ip); // name index
            name = chunk->names[index];
            scopeWithVar = nullptr;
            
            // resolve local using dynamic scope
            if (currentFrame->dynamic)
                scopeWithVar = findVarWithDynamicScaope(name, currentFrame);

            // resolve local using static scope
            if (!scopeWithVar)
                scopeWithVar = findVar(name, currentFrame->locals);

            // if does not exist
            if (!scopeWithVar)
            {
                runtimeError(cxt, "Attempted access of undefined local symbol '" + name + "'.");
            }
            
            // get callable
            scopeWithVar->get(name, a);
            b = 1;
            scopeWithVar->set(name, a - b);
            scopeWithVar->get(name, c);
            opush(currentFrame, c);
            break;
        }
		case call_instr:
		{
			// call local by register number
			i = bin.getInt(ip); // slot number
			j = bin.getInt(ip); // get nargs

			if (i > 0)
			{
				//a = currentFrame->currentScope()->registers[i];
			}
            
            
            if(cxt->errorPending)
                continue;
            
			break;
		}
		case callproperty_instr:
		{
			// call field by field number
			a = opop(currentFrame);	 // subject
			i = bin.getInt(ip); // field number
			j = bin.getInt(ip); // get nargs
			if (i > 0)
			{
			}
            
            if(cxt->errorPending)
                continue;
            
			break;
		}
		case callindex_instr: // call index
		{
			b = opop(currentFrame);	 // index
			a = opop(currentFrame);	 // subject
			j = bin.getInt(ip); // get nargs

			// read args
			std::list<hvalue> args{};
			while (j-- > 0)
				args.push_front(opop(currentFrame));

			if (!is_object(a))
            {
				runtimeError(cxt, "Subscript access not available on " + describe(a));
            }

			obj = a; // cast
			if (obj->_cxt->callIndex(_env, this, currentFrame->callee, cxt, b, args, c))
            {
                if(cxt->errorPending)
                    break;
                
				opush(currentFrame, c);
            }
			else
            {
				runtimeError(cxt, "Illegal subscript access on " + describe(a));
            }
			break;
		}
		case chain_instr: // method chaining
		{

			break;
		}
		case bnt_instr:
		{
			// bitwise NOT
			opush(currentFrame, ~opop(currentFrame));
			break;
		}
		case bnd_instr:
		{
			// bitwise AND
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a & b);
			break;
		}
		case bor_instr:
		{
			// bitwise OR
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a | b);
			break;
		}
		case bxr_instr:
		{
			// bitwise XOR
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a ^ b);
			break;
		}
		case brs_instr:
		{
			// bitwise right shift
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a >> b);
			break;
		}
		case bls_instr:
		{
			// bitwise left shift
			b = opop(currentFrame);
			a = opop(currentFrame);
			opush(currentFrame, a << b);
			break;
		}
        case cload_instr:
        {
            i = bin.getInt(ip); // slot index
            a = opop(currentFrame);
            if(!currentFrame->context)
            {
                runtimeError(cxt, "Attempting to load parameter outside of a valid context.");
                break;
            }
            currentFrame->context->params[i]->content = a;
            break;
        }
        case cstore_instr:
        {
            i = bin.getInt(ip); // slot index
            a = opop(currentFrame);
            if(!currentFrame->context)
            {
                runtimeError(cxt, "Attempting to store parameter outside of a valid context.");
                break;
            }
            opush(currentFrame, currentFrame->context->params[i]->content);
            break;
        }
        case trigger_instr:
        {
            i = bin.getInt(ip); // nargs
            a = opop(currentFrame);
            haction act = a.tryCast<HAction>();
            const VM_Action *vact = act ? act->_vaction : nullptr;
            std::map<std::string, hvalue> args;
            
            while (i-- > 0)
            {
                j = bin.getInt(ip); // name index
                name = chunk->names[i];
                b = opop(currentFrame);
                if(vact)
                    args.insert(std::pair<std::string, hvalue>(name, b));
            }
            
            if(!act)
            {
                // not an action
                runtimeError(cxt, "Attempting to trigger non-triggerable " + describe(a));
                break;
            }
            
            trigger(cxt, act, args, result);
            break;
        }
        case proceed_instr:
        {
            // proceed if context exists
            a = opop(currentFrame);
            
            if (VMContext *vmContext = currentFrame->context)
            {
                if(is_action(a))
                {
                    haction act = a;
                    act->trigger(cxt, this, vmContext);
                }
                else
                {
                    runtimeError(cxt, "Attempting to trigger non-triggerable " + describe(a) + ".");
                }
            }
            else
            {
                runtimeError(cxt, "Cannot proceed context");
            }
            break;
        }
		case callx_instr:
		{
			// call local by name
			i = bin.getInt(ip); // name index
			j = bin.getInt(ip); // get nargs
			name = chunk->names[i];
			scopeWithVar = nullptr;

			// read args
			std::list<hvalue> args{};
			while (j-- > 0)
				args.push_front(opop(currentFrame));

			// resolve callable using dynamic scope
			if (currentFrame->dynamic)
				scopeWithVar = findVarWithDynamicScaope(name, currentFrame);

			// resolve callable using static scope
			if (!scopeWithVar)
				scopeWithVar = findVar(name, currentFrame->locals);

			// if does not exist
			if (!scopeWithVar)
            {
				runtimeError(cxt, "Attempted invocation of undefined local symbol '" + name + "'.");
            }

			// get callable
			scopeWithVar->get(name, a);

			call(cxt, a, args, b);
            
            if(cxt->errorPending)
                continue;
            
			opush(currentFrame, b);
			break;
		}
		case callpropertyx_instr:
		{
			index = bin.getInt(ip);    // name index
			j = bin.getInt(ip);	  // get nargs
			name = chunk->names[index]; // property name

			// read args
			std::list<hvalue> args{};
			while (j-- > 0)
				args.push_front(opop(currentFrame));

			a = opop(currentFrame); // instance

			if (a.type() == typeid(HObject *))
			{
				obj = a;

				if (obj)
				{
					if (obj->_cxt->callProperty(_env, this, currentFrame->callee, cxt, name, args, b))
                    {
                        if(cxt->errorPending)
                            continue;
                        
						opush(currentFrame, b);
                    }
					else
					{
						// error!
                        runtimeError(cxt, "Cannot call property '" + name + "' on " + describe(obj) + ".");
					}
				}
				else
				{
					// instance is nil
					runtimeError(cxt, "Cannot call property '" + name + "' on null.");
				}
			}
			else
				runtimeError(cxt, "Cannot invoke property '" + name + "' on non-object: " + ((std::string)a));
			break;
		}
		case chainx_instr:
		{
			index = bin.getInt(ip);    // name index
			j = bin.getInt(ip);	  // get nargs
			name = chunk->names[index]; // property name

			// read args
			std::list<hvalue> args{};
			while (j-- > 0)
				args.push_front(opop(currentFrame));

			a = opop(currentFrame); // instance

			if (a.type() == typeid(HObject *))
			{
				obj = a;

				if (obj)
				{
					obj->_cxt->callProperty(_env, this, currentFrame->callee, cxt, name, args, b);

                    if(cxt->errorPending)
                        continue;
                    
				}
				else
				{
					// instance is nil
					runtimeError(cxt, "Cannot call property '" + name + "' on null.");
				}
			}
			else
            {
				runtimeError(cxt, "Cannot invoke property '" + name + "' on non-object: " + ((std::string)a));
            }
			opush(currentFrame, a); // push
			break;
		}
        case setproperty_instr:
		case setpropertyx_instr: // TODO remove
		{
			// store field by name
			i = bin.getInt(ip);    // name index
			b = opop(currentFrame);		// value
			a = opop(currentFrame);		// subject
			name = chunk->names[i]; // property name
			if (a.type() == typeid(HObject *))
			{
				obj = a;
                if (obj->_cxt->setProperty(_env, this, currentFrame->callee, cxt, name, b))
					opush(currentFrame, b);
				else
				{
					// error
					runtimeError(cxt, "Cannot write property \"" + name + "\" on " + describe(a) + "."); // cannot set property on 'x' instance
				}
			}
			else
			{
				// not an object
				runtimeError(cxt, "Cannot write \"" + name + "\" property on \"" + describe(a) + "\".");
			}
			break;
		}
        case getproperty_instr:
		case getpropertyx_instr: // TODO remove
		{
			// load field by name
			i = bin.getInt(ip);    // register number or name index
			a = opop(currentFrame);		// subject
			name = chunk->names[i]; // property name
			if (a.type() == typeid(HObject *))
			{
				obj = a;
				if (obj->_cxt->getProperty(_env, this, currentFrame->callee, cxt, name, b))
					opush(currentFrame, b);
				else
                    runtimeError(cxt, "Attempting to read undefined property \"" + name + "\" property on " + describe(a) + ".");
			}
			else
				runtimeError(cxt, "Attempting to read '" + name + "' property on non-object.");
			break;
		}
		case storex_instr:
		{
			// store local by name
			i = bin.getInt(ip);    // name index
			a = opeek(currentFrame);		// value
			name = chunk->names[i]; // local name

			scopeWithVar = nullptr;

			// resolve callable using dynamic scope
			if (currentFrame->dynamic)
				scopeWithVar = findVarWithDynamicScaope(name, currentFrame);

			// resolve callable using static scope
			if (!scopeWithVar)
				scopeWithVar = findVar(name, currentFrame->locals);

			// if does not exist
			if (scopeWithVar)
			{
				// store
				scopeWithVar->set(name, a);
			}
			else
			{
				LocalVar *var = new LocalVar{};
				var->name = name;
				var->type = new NilTypeSpecifier{};
				currentFrame->locals->define(var, a);
				//currentFrame->locals->slots[currentFrame->locals->size() - 1]->content = a; // last variable created will be the scope
			}
			break;
		}
		case loadx_instr:
		{
			// load local by name
			i = bin.getInt(ip);    // name index
			name = chunk->names[i]; // local name
			scopeWithVar = nullptr;

			// resolve callable using dynamic scope
			if (currentFrame->dynamic)
				scopeWithVar = findVarWithDynamicScaope(name, currentFrame);

			// resolve callable using static scope
			if (!scopeWithVar)
				scopeWithVar = findVar(name, currentFrame->locals);
            
			// if does not exist
			if (scopeWithVar)
			{
				scopeWithVar->get(name, a);
				opush(currentFrame, a);
			}
			else
				opush(currentFrame, undefined);

			break;
		}
		case gstorex_instr:
		{
			// store global by name
			a = opeek(currentFrame);		// value
			i = bin.getInt(ip);    // name index
			name = chunk->names[i]; // global name
			_globals->set(name, a);
			break;
		}
		case gloadx_instr:
		{
			// load global by name
			i = bin.getInt(ip);    // name index
			name = chunk->names[i]; // global name
			if (_globals->get(name, a))
				opush(currentFrame, b);
			else
				opush(currentFrame, undefined); // does not exist
			break;
		}
		case setindex_instr:
		{
			// store index
			c = opop(currentFrame); // value
			b = opop(currentFrame); // key
			a = opop(currentFrame); // subject
			if (!is_object(a))
            {
				runtimeError(cxt, "Subscript access is unavailable on " + describe(a));
            }
			obj = a; // cast
			if (obj->_cxt->setIndex(_env, this, currentFrame->callee, cxt, b, c))
				opush(currentFrame, c);
			else
				runtimeError(cxt, "Illegal subscript access on " + describe(a));
			break;
		}
		case getindex_instr:
		{
			// load index
			b = opop(currentFrame); // key
			a = opop(currentFrame); // subject
            if (!is_object(a))
            {
				runtimeError(cxt, "Subscript access not available on " + describe(a));
            }
			obj = a; // cast
			if (obj->_cxt->getIndex(_env, this, currentFrame->callee, cxt, b, c))
				opush(currentFrame, c);
			else
				runtimeError(cxt, "Illegal subscript access on " + describe(a));
			break;
		}
		case definex_instr:
		{
            i = bin.getInt(ip);
            a = chunk->constPool[i];
            
            if(a.type() == typeid(VM_Action *))
            {
                VM_Action *vact = a; // cast
                currentFrame->locals->define(vact, _env->createAction(vact));
            }
            else if(a.type() == typeid(FuncData *))
            {
                FuncData *vfunc = a; // cast
                currentFrame->locals->define(vfunc, _env->createFunction(vfunc));
            }
            else if(a.type() == typeid(VM_Class *))
            {
                //VM_Class *vclass = a; // cast
                runtimeError(cxt, "Local classes are not yet supported.");
                break;
            }
            else
			if (hfunction fn = a.tryCast<HFunction>())
			{
				// define function
				currentFrame->locals->define(fn->_vfunc, fn);
			}
            else if(haction act = a.tryCast<HAction>())
            {
                // define action
                currentFrame->locals->define(act->_vaction, act);
            }
			else if (hclass clss = a.tryCast<HClass>())
			{
				// define class
				currentFrame->locals->define(clss->_vclass, clss);
			}
			else
			{
				// not a function, class, event, etc.
				throw 1;
			}
			break;
		}
		case ocall_instr:
		{
			j = bin.getInt(ip); // nargs
            a = opop(currentFrame); // callable

			// read args
			std::list<hvalue> args{};
			while (j-- > 0)
				args.push_front(opop(currentFrame));

			call(cxt, a, args, b, false);
            
            if(cxt->errorPending)
                continue;
            
			opush(currentFrame, b);
			break;
		}
		case ret_instr:
		{
			retVal = currentFrame->halt = true;
			result = opop(currentFrame);
			break;
		}
        case throw_instr:
        {
            a = opop(currentFrame);
            //ip--; // must push back ip in order for error handler comparison to work as expected
            // get error handler
            throwError(cxt, a);
            break;
        }
        case namedcallx_instr:
        {
            // call local by index using named arguments
            index = bin.getInt(ip); // name index
            name = chunk->names[index];
            i = bin.getInt(ip); // nargs
            std::map<std::string, hvalue> args;
            
            while(i-- > 0)
            {
                j = bin.getInt(ip); // name index
                std::string n = chunk->names[j];
                b = opop(currentFrame);
                args.insert(std::pair<std::string, hvalue>(n, b));
            }
            
            scopeWithVar = nullptr;
            
            // resolve callable using dynamic scope
            if (currentFrame->dynamic)
                scopeWithVar = findVarWithDynamicScaope(name, currentFrame);

            // resolve callable using static scope
            if (!scopeWithVar)
                scopeWithVar = findVar(name, currentFrame->locals);

            // if does not exist
            if (!scopeWithVar)
            {
                runtimeError(cxt, "Attempted invocation of undefined local symbol '" + name + "'.");
            }

            // get callable
            scopeWithVar->get(name, a);
            
            c = undefined;
            
            call(cxt, a, args, c);
            
            if(cxt->errorPending)
                continue;
            
            opush(currentFrame, c);
            break;
        }
		case log_instr:
		{
			// log
			a = opop(currentFrame);
			std::cout << ((std::string)a) << std::endl;
			break;
		}
		case hlt_instr:
		{
			currentFrame->halt = true;
			break;
		}
		default:
		{
			currentFrame->halt = true;
			break;
		}
		}

        }
        catch(std::runtime_error err)
        {
            
        }
        catch(hvalue err)
        {
            
        }
        catch(hvm_halt)
        {
            
        }
        catch(hvm_abort)
        {
            // do nothing
        }
	}

	cxt->ip = currentFrame->returnAddr;
    cxt->currentFrame = currentFrame->parentFrame;
    
    // clean memory
    if(currentFrame->parentFrame == nullptr)
        while (!_opndStack.empty())
            _opndStack.pop();
    
	// garbage collection
	delete currentFrame;

	return retVal;
}

void VM::dissasemble(uint8_t opcode, uint32_t ip, Chunk *chunk)
{
	std::cout << "      " << opcode_to_string(opcode);

	switch (opcode)
	{
	case define_instr:
	case push_instr:
	case load_instr:
	case store_instr:
	case getproperty_instr:
	case setproperty_instr:
	case gload_instr:
	case gstore_instr:
	case call_instr:
	case gcall_instr:
	case ocall_instr:
	case callproperty_instr:
	{
		std::cout << " " << chunk->bytes.getInt(ip) << std::endl;
		break;
	}
	case loadx_instr:
	case storex_instr:
	case getpropertyx_instr:
	case setpropertyx_instr:
	case gloadx_instr:
	case gstorex_instr:
	case callx_instr:
	case gcallx_instr:
	case callpropertyx_instr:
	{
		std::cout << " " << chunk->names[chunk->bytes.getInt(ip)] << std::endl;
		break;
	}
	default:
	{
		std::cout << std::endl;
		break;
	}
	}
}

void VM::dumpMemory(HvmContext *cxt)
{
    auto stk = _opndStack; // clone
    int32_t i = 0;
	while (!stk.empty())
	{
        std::cout << "      op(" << i++ << ") " << _env->cast()->string(stk.top()) << std::endl;;
		stk.pop();
	}

	// restore
}

void VM::call(HvmContext *threadContext, hvalue callable, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
{
	if (is_callable(callable))
	{
        HObject *obj = callable;
        
		if (HvmCallable *callee = dynamic_cast<HvmCallable *>(obj))
		{
			// call
            result = callee->call(threadContext, this, args, thisObject);
		}
		else
        {
            hobject obj = callable; // cast
			runtimeError(threadContext, "Attempted invocation of non-callable object '" + get_qualified_name(obj->_cxt->classOf()->assemblyName()) + "'.");
        }
	}
	else if (is_undefined(callable))
		runtimeError(threadContext, "Attempted invocation of undefined.");
	else if (is_null(callable))
		runtimeError(threadContext, "Attempted invocation of null.");
    else if(is_object(callable))
    {
        hobject obj = callable; // cast
        runtimeError(threadContext, "Attempted invocation of non-callable object '" + get_qualified_name(obj->_cxt->classOf()->assemblyName()) + "'.");
    }
	else
	{
		// error: primitives are not invocable
		runtimeError(threadContext, "Attempted invocation of primitive.");
	}
}

void VM::call(HvmContext *threadContext, hvalue callable, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject)
{
    if (is_callable(callable))
    {
        HObject *obj = callable;
        
        if (HvmCallable *callee = dynamic_cast<HvmCallable *>(obj))
        {
            // call
            result = callee->call(threadContext, this, args, thisObject);
        }
        else
        {
            hobject obj = callable; // cast
            runtimeError(threadContext, "Attempted invocation of non-callable object '" + get_qualified_name(obj->_cxt->classOf()->assemblyName()) + "'.");
        }
    }
    else if (is_undefined(callable))
        runtimeError(threadContext, "Attempted invocation of undefined.");
    else if (is_null(callable))
        runtimeError(threadContext, "Attempted invocation of null.");
    else if(is_object(callable))
    {
        hobject obj = callable; // cast
        runtimeError(threadContext, "Attempted invocation of non-callable object '" + get_qualified_name(obj->_cxt->classOf()->assemblyName()) + "'.");
    }
    else
    {
        // error: primitives are not invocable
        runtimeError(threadContext, "Attempted invocation of primitive.");
    }
}

void VM::trigger(HvmContext *threadContext, hvalue triggerable, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject)
{
    if (is_action(triggerable))
    {
        haction act = triggerable; // cast
        act->trigger(threadContext, this, args, thisObject);
    }
    else if (is_undefined(triggerable))
        runtimeError(threadContext, "Attempted invocation of undefined.");
    else if (is_null(triggerable))
        runtimeError(threadContext, "Attempted invocation of null.");
    else if(is_object(triggerable))
    {
        hobject obj = triggerable; // cast
        runtimeError(threadContext, "Attempted invocation of non-callable object '" + get_qualified_name(obj->_cxt->classOf()->assemblyName()) + "'.");
    }
    else
    {
        // error: primitives are not invocable
        runtimeError(threadContext, "Attempted invocation of primitive.");
    }
}

CallFrame *VM::prepVmCall(HvmContext *threadContext, RuntimeContext *callee, Closure *closure, std::list<hvalue> &args, MemorySpace *scopeToUse, hvalue thisObject, bool useDynamicScope)
{
	// function (or callable)
    const FuncData *data = dynamic_cast<const FuncData *>(closure->data);

	if (!data)
    {
        // implementation error
		runtimeError(threadContext, "Expecting a callable object.");
        return nullptr;
    }
    
	// arity
	const uint32_t arity = data->nargs;

	// number of arguments passed
	const uint32_t nargs = (uint32_t)args.size();

	// jibber gabber
	int32_t i = 0, n;

	// array used to pass variadic arguments
	hlist vargs; // array used to pass variadic arguments to a function

	// return address
	const uint32_t returnAddr = threadContext->ip;

	// activate vm if not running
	_running = true;

	// get current call frame from thread context
	CallFrame *currentFrame = threadContext->currentFrame;

	// ensure scope
	if (!scopeToUse && currentFrame)
	{
		//scopeToUse = currentFrame->locals; // TODO get scope from closure
		scopeToUse = _globals; // TODO validate
	}

	// ensure scope exists
    MemorySpace *locals = new MemorySpace{data, scopeToUse};

	// validate argument count
	if (nargs > arity && !data->variadic)
	{
		if (!data->nativeBound)
        {
            runtimeError(threadContext, "Argument count mismatch! Received " + std::to_string(nargs) + " but expected " + std::to_string(arity) + "."); // todo argument error
        }
	}

	if (!data->nativeBound)
	{
		// iterate through each argument value
		for (hvalue argVal : args)
		{
			n = i < arity ? i : arity - 1; // argument index (max = arity)
			FuncArg *argData = data->args[n];
			TypeSpecifier *argType = argData->type;

			// type check
			if (!argType->typeCheck(argVal))
            {
				runtimeError(threadContext, "Argument '" + argData->name + "' is not a valid type."); // TODO TypeError
            }

			if (i >= arity - 1 && data->variadic)
			{
				if (!vargs)
				{
					// define variadic argument array if not defined
					vargs = _env->make()->list();
					locals->slots[n]->content = (HList *)vargs;
				}

				// push variadic argument
				vargs->setLength(vargs->getLength() + 1);
				vargs->set(vargs->getLength() - 1, argVal);
			}
			else
			{
				// define local
				locals->slots[i]->content = argVal;
			}

			i++;
		}
	}

	// populate remaining optional arguments
	for (; i < arity; i++)
	{
		FuncArg *argData = data->args[i];
		TypeSpecifier *argType = argData->type;
		InstrGroup *defaultVal = argData->defaultVal;
		hvalue argVal;

		if (defaultVal) // if argument is optional
		{
			// create call frame for argument's default value
			CallFrame *argFrame = new CallFrame{};
            argFrame->data = data;
			argFrame->chunk = data->chunk;
			argFrame->closure = closure;
			argFrame->dynamic = useDynamicScope;
			argFrame->startAddr = defaultVal->startAddr;
			argFrame->endAddr = defaultVal->endAddr;
			argFrame->thisObject = nullptr; // not applicable
			argFrame->locals = locals;	// TODO get scope properly or confirm action
			argFrame->parentFrame = currentFrame;
			argFrame->returnAddr = threadContext->ip;
            argFrame->callee = currentFrame ? currentFrame->callee : nullptr;
            
			// evaluate argument's default value
			if (cpu(threadContext, data->chunk, argFrame, argVal))
			{
				// type check
				if (!argType->typeCheck(argVal))
                {
					runtimeError(threadContext, "Argument '" + argData->name + "' is not a valid type."); // TODO TypeError
                }

				if (data->variadic)
				{
					if (!vargs)
					{
						// define variadic argument array if not defined
						vargs = _env->make()->list();
						locals->slots[i]->content = (HList *)vargs;
                        args.push_back(vargs); // push list
					}

					// push variadic argument
					vargs->setLength(vargs->getLength() + 1);
					vargs->set(vargs->getLength() - 1, argVal);
				}
				else
				{
					// define
					locals->slots[i]->content = argVal;
                    args.push_back(argVal); // push argument
				}
			}
			else // should never occur (but just in case)
            {
				runtimeError(threadContext, "An unknown error occurred when reading default argument value.");
            }
		}
		else if (data->variadic)
		{
			if (!vargs)
			{
				// define variadic argument array if not defined
				vargs = _env->make()->list();
				locals->slots[i]->content = (HList *)vargs;
                args.push_back(vargs); // push list
			}
		}
		else
        {
			runtimeError(threadContext, "Argument count mismatch! Received " + std::to_string(args.size()) + " but expected " + std::to_string(arity) + "."); // todo argument mismatch exception
        }
	}

	// create call frame
	CallFrame *frame = new CallFrame{};
    frame->data = data;
	frame->chunk = data->chunk;
	frame->parentFrame = currentFrame;
	frame->returnAddr = returnAddr;
	frame->locals = locals;
	frame->startAddr = data->startAddr;
	frame->endAddr = data->endAddr;
	frame->closure = closure;
	frame->callee = callee;

	// is dynamic scope enabled?
	frame->dynamic = useDynamicScope;

	// set thisObject on scope
	frame->thisObject = thisObject;
    
    // context is inapplicable to function calls
    frame->context = nullptr;

	// update call stack
    threadContext->currentFrame = frame;

	// return new call frame
	return frame;
}

CallFrame *VM::prepVmCall(HvmContext *threadContext, RuntimeContext *callee, Closure *closure, std::map<std::string, hvalue> &map, std::list<hvalue> &args, MemorySpace *scopeToUse, hvalue thisObject, bool useDynamicScope)
{
    // function (or callable)
    const FuncData *data = dynamic_cast<const FuncData *>(closure->data);

    if (!data)
    {
        // implementation error
        runtimeError(threadContext, "Expecting a callable object.");
        return nullptr;
    }
    
    // if not an anonymous native function
    if(!data->nativeBound)
    {
        // traverse all argument data
        for(uint16_t i = 0; i < data->nargs; i++)
        {
            auto arg = data->args[i];
            std::string argName = arg->name;
            
            // traverse map
            for(auto it = map.begin(); it != map.end(); ++it)
            {
                std::string name = it->first;
                hvalue val = it->second;
                
                if(name == argName)
                {
                    map.erase(it); // remove
                    args.push_back(val); // push argument value
                    break;
                }
            }
        }
        
        if(!map.empty())
        {
            auto it = map.begin();
            runtimeError(threadContext, "Encountered unrecognized argument '" + it->first + ".'");
        }
    }
    
    // prep vm call using created argument list
    return prepVmCall(threadContext, callee, closure, args, scopeToUse, thisObject);
}

CallFrame *VM::prepVmTrigger(HvmContext *threadContext, RuntimeContext *callee, Closure *closure, std::map<std::string, hvalue> &params, MemorySpace *scopeToUse, hvalue thisObject, bool useDynamicScope)
{
    // will only be an action
    const VM_Action *data = dynamic_cast<const VM_Action *>(closure->data);
    
    if (!data)
    {
        // implementation error
        runtimeError(threadContext, "Expecting a triggerable object.");
        return nullptr;
    }
    
    const VM_Event *contextData = data->context;
    
    // number of parameter arguments passed to create context
    //const uint32_t nargs = (uint32_t)args.size();
    
    // context size
    uint16_t nparams = 0;
    const VM_Event *contextTarget = contextData;
    
    // get context size
    while (contextTarget)
    {
        nparams += contextTarget->nparams;
        contextTarget = contextTarget->base;
    }
    
    // return address
    const uint32_t returnAddr = threadContext->ip;

    // activate vm if not running
    _running = true;

    // get current call frame from thread context
    CallFrame *currentFrame = threadContext->currentFrame;

    // ensure scope
    if (!scopeToUse && currentFrame)
    {
        //scopeToUse = currentFrame->locals; // TODO get scope from closure
        scopeToUse = _globals; // TODO validate
    }

    // ensure scope exists
    MemorySpace *locals = new MemorySpace{data, scopeToUse};
    VMContext *vmContext = new VMContext{};
    vmContext->data = contextData;
    vmContext->params = new Value*[nparams];
    
    std::vector<EventParam *> defaults;
    
    // initialize parameters
    contextTarget = contextData;
    /*
    int32_t j = nparams - 1;
    
    while(contextTarget)
    {
        // traverse parameters backwards
        for(int32_t i = nparams - 1; i >= 0; i--)
        {
            bool found = false;
            EventParam *param = contextTarget->params[i];
            
            // TODO i is NOT the correct index
            
            for(auto it = params.begin(); it != params.end(); ++it)
            {
                if(it->first == param->name)
                {
                    // store value
                    vmContext->params[j] = new Value{};
                    vmContext->params[j]->data = param;
                    vmContext->params[j]->name = it->first;
                    vmContext->params[j--]->content = it->second; // cstore
                    found = true;
                    break;
                }
            }
            
            if(!found)
            {
                // default value ?
                if(param->defaultValue)
                {
                    // remember that this parameter requires the load of a default value
                    defaults.push_back(param);
                }
                else
                {
                    // required parameter is missing
                    runtimeError(threadContext, "Missing required context parameter '" + param->name + "'.");
                }
                
            }
        }
        
        contextTarget = contextTarget->base;
    }
    */
    
    for(EventParam *paramWithDefautlValue : defaults)
    {
        hvalue value;
        
        if(cpu(threadContext, paramWithDefautlValue->defaultValue->chunk, currentFrame, value))
        {
            // TODO store default value properly
        }
    }
    
    // create call frame
    CallFrame *frame = new CallFrame{};
    frame->data = data;
    frame->chunk = data->chunk;
    frame->parentFrame = currentFrame;
    frame->returnAddr = returnAddr;
    frame->locals = locals;
    frame->startAddr = data->startAddr;
    frame->endAddr = data->endAddr;
    frame->closure = closure;
    frame->callee = callee;

    // is dynamic scope enabled?
    frame->dynamic = useDynamicScope;

    // set thisObject on scope
    frame->thisObject = thisObject;
    
    // set context
    frame->context = vmContext;

    // update call stack
    threadContext->currentFrame = frame;

    // return new call frame
    return frame;
}

HvmContext *VM::newHeap()
{
	HvmContext *cxt = new HvmContext{};
	cxt->id = _activeThreads++;
    cxt->hydroThread = new HThread{_env, _env->core()->ThreadClass()};
    cxt->errorPending = false;
    cxt->gcSafe = false;
    cxt->suspend = false;
    cxt->daemon = false;
    cxt->currentFrame = nullptr;
	return cxt;
}

hvalue VM::invokeConstructor(HvmContext *cxt, hobject instance, std::list<hvalue> &args)
{
    HClass *type = instance->_clss;
    HConstructor *construct = type->constructor();
    
    if(construct)
    {
        if(construct)
        {
            hvalue result; // redundant
            
            // call constructor
            call(cxt, construct, args, result, hvalue{instance});

            if(cxt->errorPending)
                return nullptr; // fail
        }
        else
        {
            // class is not constructible
            runtimeError(cxt, "Type '" + get_qualified_name(type->assemblyName()) + "' is not constructible.");
        }
    }
    
    return hvalue{instance};
}

hvalue VM::newInstance(HvmContext *cxt, HClass *type, std::list<hvalue> &args)
{
    // constructible?
    if(type->_constructor)
    {
        // allocate object instance
        HObject *instance = type->alloc();
        return invokeConstructor(cxt, instance, args);
    }
    
    // fail
    return nullptr;
}

template <typename ...Arg>
hvalue VM::newInstanceWithArgs(HvmContext *cxt, HClass *type, Arg...args)
{
    std::list<hvalue> constructorArgs = { args... };
    return newInstance(cxt, type, constructorArgs);
}

bool VM::exec(HvmContext *cxt, Chunk *chunk, hvalue &result, MemorySpace *scopeToUse)
{
	// if scope does not exist
	if (!scopeToUse)
		scopeToUse = _globals; // use globals

	FuncData *main = chunk->main;
	std::list<hvalue> args{};
    
    // create main function
    hfunction fn = _env->createFunction(main);
    
	// get current frame
	CallFrame *currentFrame = cxt->currentFrame;
    
	// create call frame
	CallFrame *frame = new CallFrame{};
    frame->data = main;
	frame->chunk = chunk;
	frame->parentFrame = currentFrame;
	frame->returnAddr = cxt->ip;
    frame->locals = new MemorySpace{main, scopeToUse};
	frame->startAddr = main->startAddr;
	frame->endAddr = main->endAddr;
    frame->closure = fn->_glue->closure;
	frame->dynamic = false;
	frame->thisObject = nullptr;
    frame->callee = fn;
    
	// gibber jabber
	_running = true;

    cxt->currentFrame = frame;
    
	// execution
	bool returnsVal = cpu(cxt, chunk, frame, result);

	// more gibber jabber
	_running = false;
	return returnsVal;
}

bool VM::invokeHydroFunc(HvmContext *threadContext, RuntimeContext *callingContext, Closure *closure, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
{
	// perform critical procedures for vm call
	CallFrame *frame = prepVmCall(threadContext, callingContext, closure, args, nullptr, thisObject);

	const FuncData *data = static_cast<const FuncData *>(closure->data);

	// execute
	return cpu(threadContext, data->chunk, frame, result);
}

bool VM::invokeHydroFunc(HvmContext *threadContext, RuntimeContext *callingContext, Closure *closure, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject)
{
    // perform critical procedures for vm call
    std::list<hvalue> argList;
    CallFrame *frame = prepVmCall(threadContext, callingContext, closure, args, argList, nullptr, thisObject);

    const FuncData *data = static_cast<const FuncData *>(closure->data);

    // execute
    return cpu(threadContext, data->chunk, frame, result);
}

bool VM::triggerHydroAction(HvmContext *threadContext, HAction *callingContext, Closure *closure, std::map<std::string, hvalue> &args, hvalue &result, hvalue thisObject)
{
    // perform critical procedures for vm call
    CallFrame *frame = prepVmTrigger(threadContext, callingContext, closure, args, nullptr, thisObject);
    
    const VM_Action *data = static_cast<const VM_Action *>(closure->data);

    // execute
    return cpu(threadContext, data->chunk, frame, result);
}

void VM::invokeTarget(HvmContext *threadContext, hvalue callable, std::list<hvalue> &args, hvalue &result, hvalue thisObject)
{
	call(threadContext, callable, args, result, thisObject);
}

void VM::throwError(HvmContext *cxt, hvalue error)
{
    CallFrame *frame = cxt->currentFrame;
    ErrorHandler *errorHandler = nullptr;
    
    while(frame)
    {
        if(frame->data)
        {
            for(int32_t i = 0; i < frame->data->nhandlers; i++)
            {
                ErrorHandler *err = frame->data->errorHandlers[i];
                if((frame->ipActive) >= err->fromAddr && (frame->ipActive) < err->toAddr && err->type->typeCheck(error))
                {
                    // found
                    errorHandler = err;
                    break;
                }
            }
        }
        
        if(errorHandler)
            break; // success!
        
        frame->halt = true; // kill
        frame = frame->parentFrame;
    }
    
    if(errorHandler)
    {
        // move call frame control
        cxt->errorAddr = errorHandler->startAddr;
        cxt->currentFrame = frame;
        cxt->errorPending = true;
        // TODO put local (err)
        frame->locals->slots[errorHandler->errorSlot]->content = error;
        throw hvm_abort{}; // abort action
    }
    else
    {
        cxt->currentFrame = nullptr;
        throw hvm_uncaught_error{error};
    }
}

template <typename ...Arg>
void VM::throwError(HvmContext *cxt, hclass errorType, Arg...args)
{
    std::list<hvalue> constructorArgs = { args... };
    if(hobject error = newInstance(cxt, errorType, constructorArgs))
        return throwError(cxt, hvalue{error});
}

void VM::runtimeError(HvmContext *cxt, std::string text, int32_t code)
{
    return throwError(cxt, _env->core()->RuntimeErrorClass(), text, code);
}

bool VM::getProperty(HvmContext *cxt, hobject instance, std::string name, hvalue &value)
{
    if(instance.empty())
    {
        runtimeError(cxt, "Instance must not be null.");
        return false;
    }
    
    return instance->_cxt->getProperty(_env, _env->_vm, nullptr, cxt, name, value);
}

bool VM::setProperty(HvmContext *cxt, hobject instance, std::string name, hvalue &value)
{
    if(instance.empty())
    {
        runtimeError(cxt, "Instance must not be null.");
        return false;
    }
    
    return instance->_cxt->setProperty(_env, _env->_vm, nullptr, cxt, name, value);
}

bool VM::getIndex(HvmContext *cxt, hobject instance, hvalue key, hvalue &value)
{
    if(instance.empty())
    {
        runtimeError(cxt, "Instance must not be null.");
        return false;
    }
    
    return instance->_cxt->getProperty(_env, _env->_vm, nullptr, cxt, key, value);
}

bool VM::setIndex(HvmContext *cxt, hobject instance, hvalue key, hvalue &value)
{
    if(instance.empty())
    {
        runtimeError(cxt, "Instance must not be null.");
        return false;
    }
    
    return instance->_cxt->setProperty(_env, _env->_vm, nullptr, cxt, key, value);
}

bool VM::callProperty(HvmContext *cxt, std::string name, hobject instance, std::list<hvalue> &args, hvalue &result)
{
    if(instance.empty())
    {
        runtimeError(cxt, "Instance must not be null.");
        return false;
    }
    
    return instance->_cxt->callProperty(_env, this, nullptr, cxt, name, args, result);
}

} // namespace hydro
