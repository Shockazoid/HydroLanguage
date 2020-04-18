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

#ifndef __h3o_opcodes__
#define __h3o_opcodes__

#include <cstdlib>
#include <string>

namespace hydro
{

enum hvm_isa_opcodes : uint8_t
{
	// pop
	pop_instr, // pops the operand stack

	// swap
	swp_instr, // swaps the top two operands on the operand stack

	// dup
	dup_instr, // duplicates the top of the operand stack

	// return
	ret_instr, // halts the current call frame and produces a result

	// halt
	hlt_instr, // halts the current call frame without producing a result

	// closure
	closure_instr,

	// branch
	jmp_instr, // jumps to a specific address in code memory

	// conditional branch (true)
	brt_instr, // jumps to a specific address if the top operand is true

	// conditional branch (false)
	brf_instr, // jumps to a specific address if the top operand is false

	// push constant
	push_instr,

	// positive infinity (double)
	inf_instr,

	// NaN ("Not a Number") (double)
	nan_instr,

	// true
	true_instr,

	// false
	false_instr,

	// null
	null_instr,

	// undefined
	undefined_instr,

	// load runtime definition
	rload_instr,

	// define constant (function, class, etc.)
	define_instr,

	// define constant dynamically
	definex_instr,

	// this
	this_instr,
    
    // new array new byte[2]{0b1, 0b0}
    newarray_instr,
    
    // new multidimensional array
    newmultiarray_instr,

	// object {:}
	object_instr,

	// object init property {foo:"bar"}
	objectinit_instr,

	// json {}
	json_instr,

	// json init property {"data":{"name":"Sara Donut"}}
	jsoninit_instr,

	// list []
	list_instr,

	// list push element [1, 2, true, "foo", {:}]
	listpush,

	// dictionary [:]
	dict_instr,

	// dictionary init [10:"ten"]
	dictinit_instr,

	// regular expression
	rgx_instr,

	// xml element <xml />
	xml_instr,

	// xml attribute id="followers"
	att_instr,

	// xml comment <!-- do nothing -->
	cmt_instr, // (?)

	// xml text
	txt_instr,

	// xml processing instruction <? ?>
	pin_instr,

	// xml cdata
	cdt_instr,

	// tuple ("h","i"," ","a","l","l")
	tup_instr,

	// named tuple (x:10,y:24,z:87)
	nup_instr,

	// positive (+)
	pos_instr,

	// negation (-)
	neg_instr,

	// exponential (**)
	exp_instr,

	// multiplication (*)
	mul_instr,

	// divisiion (/)
	div_instr,

	// modulo (%)
	mod_instr,

	// addition (+)
	add_instr,

	// substraction (-)
	sub_instr,
    
    // increment local (i++)
    inc_instr,
    
    // increment property (object.count++)
    incproperty_instr,
    
    // increment index (object[index]++)
    incindex_instr,
    
    // decrement local (i--)
    dec_instr,
    
    // decrement property (object.count--)
    decproperty_instr,
    
    // decrement index (object[index]--)
    decindex_instr,

	// logical equality (==)
	leq_instr,

	// strict equality (===)
	seq_instr,

	// logical inequality (!=)
	liq_instr,

	// strict inequality (!==)
	siq_instr,

	// greater than (>)
	gt_instr,

	// greater than or equal to (>=)
	gte_instr,

	// less than (<)
	lt_instr,

	// less than or equal (>)
	lte_instr,

	// logical NOT (!)
	lnt_instr,

	// logical AND (&&)
	lnd_instr,

	// logical OR (||)
	lor_instr,

	// bitwise NOT (~)
	bnt_instr,

	// bitwise AND (&)
	bnd_instr,

	// bitwise OR (|)
	bor_instr,

	// bitwise XOR (^)
	bxr_instr,

	// bitwise right shift
	brs_instr,

	// bitwise left shift
	bls_instr,

	// allocate new instance of a class on the top of the stack
	new_instr,

	// allocate new instance by name
	newx_instr,

	// allocate new instance using value from the top of the stack
	onew_instr,

	// call constructor of base class
	callsuper_instr,

	// store local
	store_instr,

	// store local by name
	storex_instr,
    
	// set property
	setproperty_instr,

	// set static property
	setstatic_instr,

	// store global
	gstore_instr,

	// set index
	setindex_instr,

	// load local
	load_instr,

	// get property
	getproperty_instr,

	// get static property
	getstatic_instr,

	// load global
	gload_instr,

	// load index
	getindex_instr,

	// load upvalue
	uload_instr,

	// store upvalue
	ustore_instr,
    
    // load parameter from current context
    cload_instr,
    
    // store parameter in current context
    cstore_instr,

	// call local
	call_instr,

	// call global
	gcall_instr,

	// call index
	callindex_instr,

	// call top operand (pops the stack)
	ocall_instr,

	// call property
	callproperty_instr,

	// call static property
	callstatic_instr,

	// chain (method chaining)
	chain_instr,

	// chain static (method chaining)
	chainstatic_instr,

	// delete dynamic local
	del_instr,

	// delete dynamic global
	gdel_instr,

	// delete dynamic field
	fdel_instr,

	// delete index (all indexes are dynamic)
	idel_instr,

	// set property by name
	setpropertyx_instr,

	// set static property by name
	setstaticx_instr,

	// store global by name
	gstorex_instr,

	// load local by name
	loadx_instr,

	// load field by name
	getpropertyx_instr,

	// load global by name
	gloadx_instr,

	// call local by name
	callx_instr,

	// call global by name
	gcallx_instr,

	// call property by name
	callpropertyx_instr,

	// chain by name (method chaining)
	chainx_instr,

	// chain static by name
	chainstaticx_instr,

	// typeof (v, d, f, b, c, object, etc.)
	typeof_instr,

	// classof (get an operands class data)
	classof_instr,

	// instanceof (type checker)
	instanceof_instr,

	// is (object only type checker)
	is_instr,

	// as
	as_instr,
    
    // fire event or other context
    fire_instr,
    
    // trigger
    trigger_instr,
    
    // proceed
    proceed_instr,
    
    // throw top of stack
    throw_instr,
    
    // call local by name using named arguments
    namedcallx_instr,
    
    // call top operand using named arguments
    namedocall_instr,
    
    // call property using named arguments
    namedcallproperty_instr,
    
    // call index using named arguments
    namedcallindex_instr,
    
	// log
	log_instr, // prints the top of the stack

	// current line within the current file source
	cln_instr, // increments the line pointer

	// current file source
	src_instr, // resets the line pointer

	// breakpoint
	bkp_instr // generates a breakpoint pausing the virtual machine
};

std::string opcode_to_string(uint8_t opcode);

} // namespace hydro

#endif /* __h3o_opcodes__ */
