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

#ifndef IntNode_hpp
#define IntNode_hpp

#include "H3oNode.hpp"

namespace hydro
{

class IntNode : public H3oNode
{
private:
    int32_t _value;
    
public:
    IntNode(int32_t i);
    virtual ~IntNode();
    virtual void build(Chunk *chunk) override;
    int32_t value() const { return _value; }
};

} // namespace hydro

#endif /* IntNode_hpp */
