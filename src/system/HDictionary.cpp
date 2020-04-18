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

#include "HDictionary.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

HDictionary::HDictionary(HvmEnv *env, HClass *dictionaryClass) : HObject{env, dictionaryClass}
{
	_size = _capacity = 0;
	_entries = new dict_entry *[0];
}

HDictionary::~HDictionary() {}

void HDictionary::construct() {}

void HDictionary::reserve(uint32_t capacity)
{
	if (capacity <= _capacity)
		return;

	dict_entry **tmp = new dict_entry *[capacity];

	uint32_t i;

	// copy
	for (i = 0; i < _size; i++)
		tmp[i] = _entries[i];

	// populate
	for (; i < capacity; i++)
		tmp[i] = nullptr;

	_capacity = capacity;
	delete[] _entries;
	_entries = tmp;
}

void HDictionary::resize(uint32_t size)
{
	// ensure capcity
	if (size > _capacity)
		reserve(size * 2);
}

bool HDictionary::tryGetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, hvalue &value)
{
	for (uint32_t i = 0; i < _size; i++)
	{
		auto entry = _entries[i];

		if (entry->key == index)
		{
			value = entry->value;
			return true;
		}
	}

	value = undefined;
	return true;
}

bool HDictionary::trySetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, const hvalue &value)
{
	for (uint32_t i = 0; i < _size; i++)
	{
		auto entry = _entries[i];

		if (entry->key == index)
		{
			entry->value = value;
			return true;
		}
	}

	// create property
	dict_entry *newEntry = new dict_entry{};
	newEntry->key = index;
	newEntry->value = value;
	resize(_size + 1);
	_entries[_size++] = newEntry;
	return true;
}

bool HDictionary::tryCallIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, std::list<hvalue> args, hvalue &result)
{
	for (uint32_t i = 0; i < _size; i++)
	{
		auto entry = _entries[i];

		if (entry->key == index)
		{
			result = env->call(entry->value, args);
			return true;
		}
	}

	// fail
	return false;
}

bool HDictionary::tryDeleteIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, const hvalue index)
{
	// TODO delete index
	return false;
}

void hydro_system_Dictionary_constructor(hdictionary instance) {}

} // namespace hydro
