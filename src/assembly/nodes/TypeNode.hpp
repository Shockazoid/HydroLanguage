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

#ifndef __h3o_TypeNode__
#define __h3o_TypeNode__

#include "H3oNode.hpp"
#include "../../vm/TypeSpecifier.hpp"

namespace hydro
{

class TypeNode : public H3oNode
{
protected:
    TypeNode();

public:
    virtual ~TypeNode();
    virtual TypeSpecifier *make() = 0;
};

} // namespace hydro

#endif /* __h3o_TypeNode__ */
