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

#include "ParamNode.hpp"

namespace hydro
{

ParamNode::ParamNode(ContextParam *vparam, InstrGroupNode *defaultValue) : _vparam{vparam}, _defaultValue{defaultValue}  {}

ParamNode::~ParamNode() {}

void ParamNode::build(Chunk *chunk)
{
    if(_defaultValue)
    {
        _defaultValue->build(chunk);
        _vparam->defaultValue = _defaultValue->group();
    }
    else
    {
        // required
        _vparam->defaultValue = nullptr;
    }
}

} // namespace hydro
