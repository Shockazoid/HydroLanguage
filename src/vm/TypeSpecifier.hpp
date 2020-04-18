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

#ifndef __h3o_TypeSpecifier__
#define __h3o_TypeSpecifier__

#include "hvalue.hpp"

namespace hydro
{

class TypeSpecifier
{
protected:
	TypeSpecifier() {}

public:
	virtual ~TypeSpecifier() {}
	virtual bool typeCheck(const hvalue &o) = 0;
};

} // namespace hydro

#endif /* __h3o_TypeSpecifier__ */
