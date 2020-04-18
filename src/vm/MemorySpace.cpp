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

#include "MemorySpace.hpp"

namespace hydro
{

MemorySpace::MemorySpace(MemorySpace *parent) : _parent{parent}
{
	_size = 0;
	slots = new Value *[0];
	assert(slots);
}

MemorySpace::MemorySpace(const FuncData *func, MemorySpace *parent) : _parent{parent}
{
	uint32_t i, j;
	Value *value;
	_size = func->nargs + func->nlocals;
	slots = new Value *[_size];
	assert(slots);

	// create slots and copy args
	for (i = 0, j = 0; j < func->nargs; i++, j++)
	{
		value = new Value{};
        assert(value);
		value->name = func->args[j]->name;
		value->content = undefined;
		value->data = func->args[j];
		value->dynamic = false;
		slots[i] = value;
	}

	// create slots and copy locals
	for (j = 0; j < func->nlocals; i++, j++)
	{
		value = new Value{};
        assert(value);
		value->name = func->locals[j]->name;
		value->content = undefined;
		value->data = func->locals[j];
		value->dynamic = false;
		slots[i] = value;
	}
}

MemorySpace::~MemorySpace()
{
	delete[] slots;
}

void MemorySpace::resize(uint32_t newSize)
{
	// TODO resize
}

void MemorySpace::define(const VMObject *data, hvalue value)
{
	for (uint32_t i = 0; i < _size; i++)
	{
		Value *v = slots[i];
		if (v->data == data)
		{
			v->content = value;
			return;
		}
	}

	Value **tmp = new Value *[_size + 1];

	// copy
	for (uint32_t i = 0; i < _size; i++)
		tmp[i] = slots[i];

	tmp[_size] = new Value{};
	tmp[_size]->name = data->name;
	tmp[_size]->data = data;
	tmp[_size]->content = value;
	tmp[_size]->dynamic = true;
	delete[] slots;
	slots = tmp;
	_size++;
}

bool MemorySpace::exists(std::string name) const
{
	for (int32_t i = 0; i < _size; i++)
		if (slots[i]->name == name)
			return true; // success!

	// does not exist
	return false;
}

bool MemorySpace::get(std::string name, hvalue &value)
{
	const uint32_t n = _size;

	// find
	for (uint32_t i = 0; i < n; i++)
	{
		if (slots[i]->name == name)
		{
			value = slots[i]->content;
			return true;
		}
	}

	// does not exist
	return false;
}

bool MemorySpace::set(std::string name, hvalue value)
{
	// find
	for (uint32_t i = 0; i < _size; i++)
	{
		if (slots[i]->name == name)
		{
			// exists
			slots[i]->content = value;
			return true;
		}
	}

	// does not exist
	return false;
}

bool MemorySpace::del(std::string name)
{
	Value **tmp = new Value *[_size - 1];
	bool result = false;
	uint32_t newSize = 0;

	// deep copy
	for (uint32_t i = 0; i < _size; i++)
	{
		if (slots[i]->dynamic && slots[i]->name == name)
		{
			// ignore copying to delete
			result = true; // property successfully deleted
		}
		else
		{
			// copy
			tmp[i] = slots[i];
			newSize++;
		}
	}

	delete[] slots;
	slots = tmp;
	_size = newSize;

	return result;
}

} // namespace hydro
