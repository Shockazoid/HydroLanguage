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

#include "EventNode.hpp"

namespace hydro
{

EventNode::EventNode(VM_Event *vevent, std::string superEventName) : BlockNode{}, _vevent{vevent}, _superEventName{superEventName} {}

EventNode::~EventNode() {}

bool EventNode::etypeExists(std::string etypeValue)
{
    for(VM_Etype *etype : _etypes)
        if(etype->name == etypeValue)
            return true; // exists!
    
    // does not exist
    return false;
}

void EventNode::appendEtype(VM_Etype *etype)
{
    _etypes.push_back(etype);
}

void EventNode::appendParam(ParamNode *param)
{
    _params.push_back(param);
}

void EventNode::build(Chunk *chunk)
{
    _vevent->nparams = (uint16_t)_params.size();
    _vevent->params = new EventParam*[_vevent->nparams];
    for(uint16_t i = 0; i < _vevent->nparams; i++)
    {
        _params[i]->build(chunk);
        _vevent->params[i] = _params[i]->vparam();
    }
    _vevent->netypes = (uint16_t)_etypes.size();
    _vevent->etypes = new VM_Etype*[_vevent->netypes];
    for(uint16_t i = 0; i < _vevent->netypes; i++)
        _vevent->etypes[i] = _etypes[i];
}

} // namespace hydro
