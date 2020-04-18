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

#ifndef __h3o_FuncArgSymbol__
#define __h3o_FuncArgSymbol__

#include "VarSymbol.hpp"

namespace hydro
{

class FuncArgSymbol : public VarSymbol
{
public:
	FuncArgSymbol(Name *name, TypeSpec *type) : VarSymbol{name, type} {}
	virtual ~FuncArgSymbol() {}
};

} // namespace hydro

#endif /* __h3o_FuncArgSymbol__ */
