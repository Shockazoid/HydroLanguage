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

#include "opcodes.hpp"

namespace hydro
{

std::string opcode_to_string(uint8_t opcode)
{
	switch (opcode)
	{
	case pop_instr:
		return "pop";
	case swp_instr:
		return "swap";
	case dup_instr:
		return "dup";
	case ret_instr:
		return "ret";
	case hlt_instr:
		return "hlt";
	case closure_instr:
		return "closure";
	case jmp_instr:
		return "jmp";
	case brt_instr:
		return "brt";
	case brf_instr:
		return "brf";
	case push_instr:
		return "push";
	case inf_instr:
		return "inf";
	case nan_instr:
		return "nan";
	case true_instr:
		return "true";
	case false_instr:
		return "false";
	case null_instr:
		return "nil";
	case undefined_instr:
		return "und";
	case this_instr:
		return "this";
	case rload_instr:
		return "loadclass";
	case object_instr:
		return "object";
	case json_instr:
		return "json";
	case list_instr:
		return "array";
	case listpush:
		return "arraypush";
	case dict_instr:
		return "dict";
	case rgx_instr:
		return "regex";
	case xml_instr:
		return "xml";
	case att_instr:
		return "attr";
	case cmt_instr:
		return "com";
	case txt_instr:
		return "txt";
	case pin_instr:
		return "pin";
	case cdt_instr:
		return "cdt";
	case tup_instr:
		return "tup";
	case nup_instr:
		return "nup";
	case pos_instr:
		return "pos";
	case neg_instr:
		return "neg";
	case exp_instr:
		return "exp";
	case mul_instr:
		return "mul";
	case div_instr:
		return "div";
	case mod_instr:
		return "mod";
	case add_instr:
		return "add";
	case sub_instr:
		return "sub";
	case leq_instr:
		return "leq";
	case liq_instr:
		return "liq";
	case seq_instr:
		return "seq";
	case siq_instr:
		return "siq";
	case gt_instr:
		return "gt";
	case gte_instr:
		return "gte";
	case lt_instr:
		return "lt";
	case lte_instr:
		return "lte";
	case lnt_instr:
		return "lnt";
	case lnd_instr:
		return "lnd";
	case lor_instr:
		return "lor";
	case bnt_instr:
		return "bnt";
	case bnd_instr:
		return "bnd";
	case bor_instr:
		return "bor";
	case bxr_instr:
		return "bxr";
	case brs_instr:
		return "brs";
	case bls_instr:
		return "bls";
	case new_instr:
		return "new";
	case store_instr:
		return "store";
	case storex_instr:
		return "storex";
	case load_instr:
		return "load";
	case loadx_instr:
		return "loadx";
	case getindex_instr:
		return "iload";
	case setproperty_instr:
		return "setproperty";
	case setstatic_instr:
		return "setstatic";
	case setpropertyx_instr:
		return "setpropertyx";
	case setstaticx_instr:
		return "setstaticx";
	case getproperty_instr:
		return "getproperty";
	case getpropertyx_instr:
		return "getpropertyx";
	case gstore_instr:
		return "gstore";
	case gstorex_instr:
		return "gstorex";
	case gload_instr:
		return "gload";
	case gloadx_instr:
		return "gloadx";
	case setindex_instr:
		return "istore";
	case uload_instr:
		return "upload";
	case ustore_instr:
		return "upstore";
	case call_instr:
		return "call";
	case callx_instr:
		return "callx";
	case callproperty_instr:
		return "callproperty";
	case callpropertyx_instr:
		return "callpropertyx";
	case callstatic_instr:
		return "callstatic";
	case callindex_instr:
		return "icall";
	case ocall_instr:
		return "ocall";
	case chain_instr:
		return "chain";
	case chainx_instr:
		return "chainx";
	case typeof_instr:
		return "typeof";
	case classof_instr:
		return "classof";
	case instanceof_instr:
		return "instanceof";
	case is_instr:
		return "is";
	case as_instr:
		return "as";
	case define_instr:
		return "define";
	case definex_instr:
		return "definex";
    case fire_instr:
        return "fire";
	default:
	{
		// do nothing
		break;
	}
	}

	return "(unknown opcode)";
}

} // namespace hydro
