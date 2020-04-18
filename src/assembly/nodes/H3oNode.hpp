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

#ifndef __h3o_H3oNode__
#define __h3o_H3oNode__

#include "../../parser/__ast/Ast.hpp"
#include "../../vm/Chunk.hpp"

namespace hydro
{

class H3oNode : public Ast
{
protected:
    void buildChildren(Chunk *chunk);

public:
	H3oNode();
	virtual ~H3oNode();
    virtual void build(Chunk *chunk);
};

} // namespace hydro

#endif /* __h3o_H3oNode__ */
