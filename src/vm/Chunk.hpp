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

#ifndef __h3o_Chunk__
#define __h3o_Chunk__

#include "hvalue.hpp"
#include "bytearray.hpp"

namespace hydro
{

struct Chunk
{
	uint32_t size;
    bytearray bytes;
	std::string *names{nullptr};
	hvalue *constPool{nullptr};
	//class MemorySpace *globals{nullptr};
	class Runtime *runtime{nullptr};
	class FuncData *main{nullptr};
#ifdef HVM_DEBUG
	uint32_t *lines{nullptr};
#endif /* HVM_DEBUG */
};

} // namespace hydro

#endif /* __h3o_Chunk__ */
