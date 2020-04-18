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

#ifndef __h3o_FuncArg__
#define __h3o_FuncArg__

#include "LocalVar.hpp"
#include "InstrGroup.hpp"

namespace hydro
{

struct FuncArg final : public LocalVar
{
	InstrGroup *defaultVal{nullptr};
	virtual ~FuncArg() {}
};

} // namespace hydro

#endif /* __h3o_FuncArg__ */
