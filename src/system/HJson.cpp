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

#include "HJson.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

HJson::HJson(HvmEnv *env, HClass *jsonClass) : HObject{env, jsonClass}
{
	_size = 0;
	_props = new json_property *[0];
}

HJson::~HJson()
{
	delete[] _props;
}

void HJson::construct(hvalue dataArg)
{
	// TODO initialize Json
}

std::string HJson::jsonEncodeString(std::string rawString)
{
	std::string s = "";

	for (char ch : rawString)
	{
		switch (ch)
		{
		case '\n':
			s += "\\n";
			break;
		case '\f':
			s += "\\f";
			break;
		case '\t':
			s += "\\t";
			break;
		case '\r':
			s += "\\r";
			break;
		case '"':
			s += "\\\"";
			break;
		case '\\':
			s += "\\\\";
			break;
		default:
		{
			s += ch;
			break;
		}
		}
	}

	return '"' + s + '"';
}

std::string HJson::jsonEncodeValue(HvmEnv *env, hvalue jsonValue)
{
	if (is_string(jsonValue))
		return jsonEncodeString(env->cast()->string(jsonValue));
	else if (is_double(jsonValue))
	{
		double d = jsonValue; // cast
		std::string sign = "";
		if (d < 0)
			sign = "-"; // negative
		return sign + env->cast()->string(jsonValue);
	}
	else if (is_bool(jsonValue))
		return (std::string)jsonValue;
	else if (is_null(jsonValue))
		return "null";
	else if (is_list(jsonValue))
	{
		hlist list = jsonValue; // cast
		std::string s = "";
		auto len = list->getLength();

		for (uint32_t i = 0; i < len; i++)
		{
			hvalue element = list->get(i);
			if (i > 0)
				s += ", ";
			s += jsonEncodeValue(env, element);
		}

		return "[" + s + "]";
	}
	else if (is_object(jsonValue))
	{
		hjson json = jsonValue; // cast (will be json)
		std::string s = "";

		for (uint16_t i = 0; i < json->_size; i++)
		{
			auto prop = json->_props[i];

			if (i > 0)
				s += ", ";

			s += jsonEncodeString(prop->key);
			s += ":";
			s += jsonEncodeValue(env, prop->value);
		}

		return "{" + s + "}";
	}

	// fail
	return "";
}

void HJson::ensureCapacity(uint16_t capacity)
{
	if (capacity <= _capacity)
		return;

	json_property **tmp = new json_property *[capacity];

	uint16_t i;

	// copy
	for (i = 0; i < _size; i++)
		tmp[i] = _props[i];

	// populate
	for (; i < capacity; i++)
		tmp[i] = nullptr;

	_capacity = capacity;
	delete[] _props;
	_props = tmp;
}

bool HJson::tryGetProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name, hvalue &value)
{
	for (uint16_t i = 0; i < _size; i++)
	{
		auto prop = _props[i];

		if (prop->key == name)
		{
			value = prop->value;
			return true;
		}
	}

	// fail
	value = undefined;
	return true;
}

bool HJson::trySetProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name, const hvalue &value)
{
	// TODO restrict to JSON values

	for (uint16_t i = 0; i < _size; i++)
	{
		auto prop = _props[i];

		if (prop->key == name)
		{
			prop->value = value;
			return true;
		}
	}

	// create property
	json_property *newProp = new json_property{};
	newProp->key = name;
	newProp->value = value;
	ensureCapacity(_size + 1);
	_props[_size++] = newProp;
	return true;
}

bool HJson::tryGetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, hvalue &value)
{
	if (!is_string(index))
		throw std::runtime_error{"JSON index must be a string."};

	std::string key = env->cast()->string(index);

	for (uint16_t i = 0; i < _size; i++)
	{
		auto prop = _props[i];

		if (prop->key == key)
		{
			value = prop->value;
			return true;
		}
	}

	// fail
	value = undefined;
	return true;
}

bool HJson::trySetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, const hvalue &value)
{
	if (!is_string(index))
		throw std::runtime_error{"JSON index must be a string."};

	// TODO restrict to JSON values

	std::string key = env->cast()->string(index);

	for (uint16_t i = 0; i < _size; i++)
	{
		auto prop = _props[i];

		if (prop->key == key)
		{
			prop->value = value;
			return true;
		}
	}

	// create property
	json_property *newProp = new json_property{};
	newProp->key = key;
	newProp->value = value;
	ensureCapacity(_size + 1);
	_props[_size++] = newProp;
	return true;
}

bool HJson::tryDeleteProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name)
{
	json_property **tmp = new json_property *[_capacity];
	bool result = false;

	for (uint16_t i = 0; i < _capacity; i++)
	{
		auto prop = _props[i];

		if (!prop)
			break; // end

		if (prop->key != name)
			tmp[result ? i - 1 : i] = prop;
		else
		{
			result = true; // success!
			delete prop;
		}
	}

	// does not exist (no error)
	return result;
}

bool HJson::tryDeleteIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index)
{
	if (!is_string(index))
		throw std::runtime_error{"JSON index must be a string."};

	std::string key = env->cast()->string(index);
	json_property **tmp = new json_property *[_capacity];
	bool result = false;

	for (uint16_t i = 0; i < _capacity; i++)
	{
		auto prop = _props[i];

		if (!prop)
			break; // end

		if (prop->key != key)
			tmp[result ? i - 1 : i] = prop;
		else
		{
			result = true; // success!
			delete prop;
		}
	}

	// does not exist (no error)
	return result;
}

hjson HJson::parse(HvmEnv *env, std::string jsonString, hfunction reviver)
{
	hjson json = env->make()->json();
	// TODO parse JSON string
	return json;
}

std::string HJson::toString()
{
	return jsonEncodeValue(_env, this);
}

} // namespace hydro
