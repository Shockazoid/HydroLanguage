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

#include "SetterNode.hpp"

namespace hydro
{

SetterNode::SetterNode(PropertyNode *owner, SetterData *vsetter) : FuncNode{vsetter}, _owner{owner}, _vsetter{vsetter} {}

SetterNode::~SetterNode() {}

void SetterNode::build(Chunk *chunk)
{
    if(_args.size() > 1)
    {
        throw std::runtime_error{"Setter defined with too many arguments."}; // TODO compiler error
    }
    else if (_args.size() == 0)
    {
        FuncArg *varg = new FuncArg{};
        varg->defaultVal = nullptr;
        varg->modifier = 0;
        varg->name = "value";
        varg->type = new NilTypeSpecifier{};
        _args.push_back(new FuncArgNode{varg});
    }
    FuncNode::build(chunk);
    _vsetter->property = _owner->property();
    _vsetter->owner = _owner->property()->owner;
    _owner->property()->setter = _vsetter;
}

} // namespace hydro
