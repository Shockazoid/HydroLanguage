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

#ifndef __h3o_NilTypeSpecifier__
#define __h3o_NilTypeSpecifier__

#include "TypeSpecifier.hpp"

namespace hydro
{

class NilTypeSpecifier : public TypeSpecifier
{
public:
	NilTypeSpecifier() : TypeSpecifier{} {}
	virtual ~NilTypeSpecifier() {}
	virtual bool typeCheck(const hvalue &o) override { return true; }
};

} // namespace hydro

#endif /* __h3o_NilTypeSpecifier__ */
