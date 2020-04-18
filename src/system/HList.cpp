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

#include "HList.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

HList::HList(HvmEnv *env, hclass vclass) : HObject{env, vclass}, _len{0}, _buffer{new hvalue[0]} {}

HList::~HList()
{
	delete[] _buffer;
}

void HList::construct()
{
    
}

bool HList::tryGetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, hvalue &value)
{
	hvalue a = 0, b = _len;

	if (index >= a && index < b)
	{
		int32_t i = index;
		value = _buffer[i];
		return true;
	}
	else
	{
		// TODO index out of bounds
	}

	return false;
}

bool HList::trySetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, const hvalue &value)
{
	hvalue a = 0, b = _len;

	if (index >= a && index < b)
	{
		int32_t i = index;
		_buffer[i] = value;
		return true;
	}
	else
	{
		// TODO index out of bounds
	}

	return false;
}

bool HList::tryCallIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, std::list<hvalue> args, hvalue &result)
{
	hvalue a = 0, b = _len;

	if (index >= a && index < b)
	{
		int32_t i = index;
		result = env->call(_buffer[i], args);
		return true;
	}
	else
	{
		// TODO index out of bounds
	}

	return false;
}

void HList::set(uint32_t index, hvalue element)
{
	_buffer[index] = element;
}

uint32_t HList::push(hvalue element)
{
	setLength(_len + 1);		// increase size
	set(_len - 1, element); // push
	return _len;
}

hvalue HList::pop()
{
    if (_len == 0)
        return undefined;
    
    // pop
    hvalue top = _buffer[_len];
    _len--;
    _buffer[_len] = undefined;
    return top;
}

void HList::setLength(uint32_t value)
{
	if (value == _len)
		return; // abort

	hvalue *tmp = new hvalue[value];

	uint32_t i = 0;

	// copy
	for (; i < value && i < _len; i++)
		tmp[i] = _buffer[i];

	// populate
	for (; i < value; i++)
		tmp[i] = undefined;

	// resize
	delete[] _buffer;
	_buffer = tmp;
	_len = value;
}

std::string HList::toString()
{
	std::string s = "";
	for (uint32_t i = 0; i < _len; i++)
	{
		std::string n = _env->cast()->string(_buffer[i]);
		if (i > 0)
			s += ", " + n;
		else
			s += n;
	}

	return "[" + s + "]";
}

void hydro_system_List_constructor(hlist instance, hlist elements)
{
	const uint32_t len = elements->getLength();
	instance->setLength(len); // set length

	// copy
	for (uint32_t i = 0; i < len; i++)
		instance->_buffer[i] = elements->_buffer[i];
}

hlist HList::clone() const
{
	hlist cpy = _env->make()->list();

	// set length
	cpy->setLength(_len);

	// deep copy
	for (uint32_t i = 0; i < _len; i++)
		cpy->_buffer[i] = _buffer[i];

	return cpy;
}

} // namespace hydro
