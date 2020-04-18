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

#include "../vm/HvmEnv.hpp"
#include "HClass.hpp"
#include "HConstructor.hpp"
#include "HProperty.hpp"
#include "HMethod.hpp"

namespace hydro
{

HClass::HClass(class HvmEnv *env, const VM_Class *vclass, ClassAllocator *allocator) : _fields{new RuntimeContext *[0]}, _vclass{vclass}, RuntimeContext{get_package_name(vclass->name)}, _superClass{nullptr}, HObject{env, this, this}, _allocator{allocator}, _constructor{nullptr}
{
    // late initialization will be applied by HvmEnv
}

HClass::HClass(class HvmEnv *env, HClass *classHClass, const VM_Class *vclass, ClassAllocator *allocator) : HObject{env, classHClass, this}, RuntimeContext{get_package_name(vclass->name)}, _vclass{vclass}, _superClass{nullptr}, _allocator{allocator}, _constructor{nullptr}
{
    // late initialization will be applied by HvmEnv
}

HClass::HClass(HvmEnv *env, HClass *classClass, const VM_Class *vclass, HClass *superClass, ClassAllocator *allocator) : HObject{env, classClass, this}, RuntimeContext{get_package_name(vclass->name)}, _vclass{vclass}, _superClass{superClass}, _allocator{allocator}, _constructor{nullptr}
{
    setup();
}

HClass::~HClass()
{
}

void HClass::setup()
{
    HObject::setup(this);
    auto info = get_class_info(_vclass->name);
    
    if(info)
    {
        _name = info->name;
        _qualifiedName = info->qualifiedName;
        _packageName = info->packageName;
    }
    
    _fields = new RuntimeContext*[_vclass->nfields];
    
    if(_vclass->construct)
        _constructor = _env->createConstructor(_vclass->construct, this);
    
    for(uint16_t i = 0; i < _vclass->nfields; i++)
    {
        if(PropertyData *prop = dynamic_cast<PropertyData *>(_vclass->fields[i]))
        {
            // bind property
            _fields[i] = _env->createProperty(prop, this);
        }
        else if(MethodData *meth = dynamic_cast<MethodData *>(_vclass->fields[i]))
        {
            // bind method
            _fields[i] = _env->createMethod(meth, this);
        }
        else
        {
            // error!
            throw std::runtime_error{"Unsupported field type."};
        }
    }
}

HObject *HClass::alloc()
{
    ClassAllocator *allocator = nullptr;
    HClass *clss = this;
    
    while(!allocator && clss)
    {
        allocator = clss->_allocator;
        clss = _superClass;
    }
    
    if(!allocator)
        return new HObject{_env, this};
    
    return allocator->alloc(_env, this);
}

uint16_t HClass::countFields()
{
	uint16_t n = 0;

	HClass *clss = this;

	while (clss)
	{
		for (uint16_t i = 0; i < clss->_vclass->nfields; i++)
		{
			if (!is_override(clss->_fields[i]->modifier()))
				n++;
		}

		clss = clss->_superClass;
	}

	return n;
}

RuntimeContext *HClass::lookup(std::string name) const
{
	// find property by name
	for (uint32_t i = 0; i < _vclass->nfields; i++)
	{
		RuntimeContext *f = _fields[i];
		if (f->assemblyName() == name)
			return f;
	}

	// fail
	return nullptr;
}

std::string HClass::toString()
{
	return "[class " + get_simple_name(_vclass->name) + "]";
}

} // namespace hydro
