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

#ifndef __h3o_ObjectTypeNode__
#define __h3o_ObjectTypeNode__

#include "TypeNode.hpp"
#include "../../vm/ObjectTypeSpecifier.hpp"

namespace hydro
{

class ObjectTypeNode : public TypeNode
{
public:
    ObjectTypeNode();
    virtual ~ObjectTypeNode();
    virtual void build(Chunk *chunk) override;
    virtual TypeSpecifier *make() override;
};

} // namespace hydro

#endif /* __h3o_ObjectTypeNode__ */
