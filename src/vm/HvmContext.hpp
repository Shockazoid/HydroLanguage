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

#ifndef __h3o_HvmContext__
#define __h3o_HvmContext__

#include "hvalue.hpp"
#include "../system/HThread.hpp"

namespace hydro
{

// The <code>HvmContext</code> represents a thread context.
struct HvmContext
{
    bool errorPending{false};
    class CallFrame *currentFrame{nullptr}; // stack pointer
	int64_t sp{0}; // stack pointer
	uint32_t ip{0}; // instruction pointer
	void *osThread{nullptr};
	HThread *hydroThread{nullptr};
	uint32_t id{0};
	volatile int32_t status{0};
	int32_t priotity{0};
    uint32_t errorAddr{0};
	bool daemon{false};
	bool gcSafe{false};
	bool suspend{false}; // to suspend ?
};

} // namespace hydro

#endif /* __h3o_HvmContext__ */
