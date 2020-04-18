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

#ifndef __h3o_CallFrame__
#define __h3o_CallFrame__

#include <list>
#include <cstdlib>
#include <vector>

#include "Chunk.hpp"
#include "VM_Func.hpp"
#include "Closure.hpp"
#include "RuntimeContext.hpp"
#include "EventContext.hpp"

namespace hydro
{

struct CallFrame
{
	Chunk *chunk;
	CallFrame *parentFrame{nullptr};
	Closure *closure{nullptr};
    EventContext *context{nullptr};
	const VM_Func *data{nullptr};
    RuntimeContext *callee;
	MemorySpace *locals;
	uint32_t returnAddr;
	uint32_t startAddr;
	uint32_t endAddr;
    uint32_t ip;
    uint32_t ipActive; // address to current executing opcode (used for error handling)
    std::vector<hvalue> operands;
	bool dynamic{false}; // is dynamic scope enabled?
	bool halt{false};
	bool trace{false};
	hvalue thisObject{nullptr};
};

} // namespace hydro

#endif /* __h3o_CallFrame__ */
