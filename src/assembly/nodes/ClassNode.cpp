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

#include "ClassNode.hpp"
#include "VM_Method.hpp"
#include "ConstructorNode.hpp"

namespace hydro
{

ClassNode::ClassNode(Chunk *chunk, VM_Class *vclass, std::string superClassName) : _chunk{chunk}, _vclass{vclass}, _super{superClassName}, _fields{}, _construct{nullptr} {}

ClassNode::~ClassNode() {}

void ClassNode::build(Chunk *chunk)
{
    _vclass->nfields = (uint16_t)_fields.size();
    _vclass->fields = new VM_Object*[_vclass->nfields];
    _vclass->ninstance = 0;
    _vclass->nstatic = 0;
    
    int32_t i = 0;
    
    // emit constructor
    if(_construct)
        _construct->build(chunk);
    
    // emit fields
    for(H3oNode *node : _fields)
    {
        // build
        node->build(chunk);
        
        if(PropertyNode *prop = dynamic_cast<PropertyNode *>(node))
        {
            // bind property
            _vclass->fields[i] = prop->property();
            if(is_static(prop->property()->modifier))
            {
                _vclass->nstatic++;
            }
            else
            {
                _vclass->ninstance++;
                if(!_construct && !is_native(_vclass->modifier))
                    throw std::runtime_error{"Cannot initialize instance properties on object of type '" + _vclass->name + "'."};
            }
        }
        else if(MethodNode *meth = dynamic_cast<MethodNode *>(node))
        {
            // bind method
            _vclass->fields[i] = meth->method();
            if(is_static(meth->method()->modifier))
                _vclass->nstatic++;
        }
        
        i++;
    }
}

void ClassNode::addProperty(PropertyNode *prop)
{
	_fields.push_back(prop);
}

void ClassNode::addMethod(MethodNode *meth)
{
	_fields.push_back(meth);
}

void ClassNode::setConstructor(ConstructorNode *construct)
{
    _construct = construct;
}

} // namespace hydro
