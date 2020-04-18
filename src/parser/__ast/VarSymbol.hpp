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

#ifndef __h3o_VarSymbol__
#define __h3o_VarSymbol__

#include "TypeSymbol.hpp"

namespace hydro
{

class VarSymbol : public Symbol
{
private:
	TypeSpec *_type;

public:
	VarSymbol(Name *name, TypeSpec *type) : Symbol{nullptr, name}, _type{type} {}
	virtual ~VarSymbol() {}
	TypeSpec *type() const { return _type; }
};

} // namespace hydro

#endif /* __h3o_VarSymbol__ */
