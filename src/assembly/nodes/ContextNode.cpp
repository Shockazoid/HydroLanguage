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

#include "ContextNode.hpp"

namespace hydro
{

ContextNode::ContextNode(ContextData *vcontext) : _vcontext{vcontext} {}

ContextNode::~ContextNode() {}

void ContextNode::appendParam(ParamNode *param)
{
    _params.push_back(param);
}

void ContextNode::build(Chunk *chunk)
{
    _vcontext->nparams = (uint16_t)_params.size();
    _vcontext->params = new EventParam*[_vcontext->nparams];
    for(uint16_t i = 0; i < _vcontext->nparams; i++)
    {
        _params[i]->build(chunk);
        _vcontext->params[i] = _params[i]->vparam();
    }
}

} // namespace hydro
