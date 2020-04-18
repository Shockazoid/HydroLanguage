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

#ifndef __h3o_ContextParam__
#define __h3o_ContextParam__

#include "VMObject.hpp"
#include "TypeSpecifier.hpp"
#include "InstrGroup.hpp"

namespace hydro
{

struct ContextParam : public VMObject
{
    TypeSpecifier *type;
    InstrGroup *defaultValue;
};

} // namespace hydro

#endif /* __h3o_ContextParam__ */
