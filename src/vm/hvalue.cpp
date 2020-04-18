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

#include <type_traits>

#include "hvalue.hpp"
#include "Closure.hpp"
#include "../system/HObject.hpp"
#include "../system/HContext.hpp"
#include "../system/HEventContext.hpp"
#include "../system/HString.hpp"
#include "../system/HList.hpp"
#include "../system/HFunction.hpp"
#include "../system/HAction.hpp"
#include "../system/HList.hpp"
#include "../system/HDate.hpp"
#include "../system/HError.hpp"
#include "../system/HMethod.hpp"
#include "../system/HConstructor.hpp"
#include "../system/HProperty.hpp"
#include "../system/HClass.hpp"
#include "../memory/object_ptr.hpp"

namespace hydro
{

hvalue::hvalue() : content{new holder<undefined_t>{undefined}} {}

hvalue::hvalue(undefined_t o) : content{new holder<undefined_t>{o}} {}

hvalue::hvalue(std::nullptr_t o) : content{new holder<std::nullptr_t>{o}} {}

hvalue::hvalue(const hvalue &o) : content{o.content->clone()} {}

hvalue::hvalue(hvalue &&o) : content{std::move(o.content)} {}

hvalue::hvalue(uint8_t o) : content{new holder<uint8_t>{o}} {}

hvalue::hvalue(int8_t o) : content{new holder<int8_t>{o}} {}

hvalue::hvalue(uint16_t o) : content{new holder<uint16_t>{o}} {}

hvalue::hvalue(int16_t o) : content{new holder<int16_t>{o}} {}

hvalue::hvalue(uint32_t o) : content{new holder<uint32_t>{o}} {}

hvalue::hvalue(int32_t o) : content{new holder<int32_t>{o}} {}

hvalue::hvalue(uint64_t o) : content{new holder<uint64_t>{o}} {}

hvalue::hvalue(int64_t o) : content{new holder<int64_t>{o}} {}

hvalue::hvalue(float o) : content{new holder<float>{o}} {}

hvalue::hvalue(double o) : content{new holder<double>{o}} {}

hvalue::hvalue(char o) : content{new holder<char16_t>{(char16_t)o}} {}

hvalue::hvalue(char16_t o) : content{new holder<char16_t>{o}} {}

hvalue::hvalue(bool o) : content{new holder<bool>{o}} {}

hvalue::hvalue(std::string o) : content{new holder<HObject *>{(HObject *)new HString{o}}} { hold(); }

hvalue::hvalue(const char *o) : content{new holder<HObject *>{(HObject *)new HString{o}}} { hold(); }

hvalue::hvalue(HObject *o) : content{new holder<HObject *>{o}} { hold(); }

hvalue::~hvalue() {}

void hvalue::hold() const
{
	if (content && content->type() == typeid(HObject *))
	{
		HObject *obj = static_cast<holder<HObject *> *>(content.get())->value;
		if (obj)
			obj->addRef();
	}
}

void hvalue::release() const
{
	if (content && content->type() == typeid(HObject *))
	{
		HObject *obj = static_cast<holder<HObject *> *>(content.get())->value;
		if (obj)
			obj->removeRef();
	}
}

template <typename T>
bool hvalue::convert(hvalue &value, T &result)
{
	if (value.type() == typeid(T))
	{
		result = (T)value;
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, std::string &result)
{
    if(hstring s = value.tryCast<HString>())
    {
        result = s->toString();
        return true;
    }
	else if (value.type() == typeid(std::nullptr_t))
	{
		result = "null";
		return true;
	}
	else if (value.type() == typeid(undefined_t))
	{
		result = "undefined";
		return true;
	}
	else if (value.type() == typeid(bool))
	{
		result = ((bool)value) ? "true" : "false";
		return true;
	}
	else if (value.type() == typeid(char16_t))
	{
		result = std::string{1, static_cast<char>((char16_t)value)};
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = std::to_string((uint8_t)(value));
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = std::to_string((int8_t)value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = std::to_string((uint16_t)value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = std::to_string((int16_t)value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = std::to_string((uint32_t)value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = std::to_string((int32_t)value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = std::to_string((uint64_t)value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = std::to_string((int64_t)value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		float f = value; // cast float
		result = std::to_string(f);
		result.erase(result.find_last_not_of('0') + 1, std::string::npos);
		if (std::isnan(f))
			result = "NaN";
		else if (f == std::numeric_limits<float>::infinity())
			result = "Infinity";
		else if (f == (-std::numeric_limits<float>::infinity()))
			result = "-Infinity";
		else if (result[result.length() - 1] == '.')
			result = result.substr(0, result.length() - 1);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		double d = value; // cast double

		if (std::isnan(d))
			result = "NaN";
		else if (d == std::numeric_limits<double>::infinity())
			result = "Infinity";
		else if (d == (-std::numeric_limits<double>::infinity()))
			result = "-Infinity";
		else
		{
			result = std::to_string(d);
			result.erase(result.find_last_not_of('0') + 1, std::string::npos);
			if (result[result.length() - 1] == '.')
				result = result.substr(0, result.length() - 1);
		}

		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, bool &result)
{
	if (convert<bool>(value, result))
		return true;

	int32_t n;

	if (convert(value, n))
	{
		result = (n != 0);
		return true;
	}

	std::string str;

	if (convert(value, str))
	{
		result = (str.length() > 0);
		return true;
	}

	// fail
	return false;
}


bool hvalue::convert(hvalue &value, char &result)
{
    if (convert<char>(value, result))
        return true;

    if (value.type() == typeid(uint8_t))
    {
        result = (char)(uint8_t)(value);
        return true;
    }
    else if (value.type() == typeid(int8_t))
    {
        result = (char)(int8_t)(value);
        return true;
    }
    else if (value.type() == typeid(uint16_t))
    {
        result = (char)(uint16_t)(value);
        return true;
    }
    else if (value.type() == typeid(int16_t))
    {
        result = (char)(int16_t)(value);
        return true;
    }
    else if (value.type() == typeid(uint32_t))
    {
        result = (char)(uint32_t)(value);
        return true;
    }
    else if (value.type() == typeid(int32_t))
    {
        result = (char)(int32_t)(value);
        return true;
    }
    else if (value.type() == typeid(uint64_t))
    {
        result = (char)(uint64_t)(value);
        return true;
    }
    else if (value.type() == typeid(int64_t))
    {
        result = (char)(int64_t)(value);
        return true;
    }
    else if (value.type() == typeid(float))
    {
        result = (char)(float)(value);
        return true;
    }
    else if (value.type() == typeid(double))
    {
        result = (char)(double)(value);
        return true;
    }

    // fail
    return false;
}

bool hvalue::convert(hvalue &value, char16_t &result)
{
	if (convert<char16_t>(value, result))
		return true;

	if (value.type() == typeid(uint8_t))
	{
		result = (char16_t)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (char16_t)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (char16_t)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (char16_t)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (char16_t)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (char16_t)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (char16_t)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (char16_t)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (char16_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (char16_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, const char *&result)
{
	std::string str;

	if (convert<std::string>(value, str))
	{
		result = str.c_str();
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, uint8_t &result)
{
	if (convert<uint8_t>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (uint8_t)(char)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (uint8_t)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (uint8_t)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (uint8_t)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (uint8_t)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (uint8_t)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (uint8_t)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (uint8_t)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (uint8_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (uint8_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, int8_t &result)
{
	if (convert<int8_t>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (int8_t)(char)(value);
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = (int8_t)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (int8_t)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (int8_t)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (int8_t)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (int8_t)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (int8_t)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (int8_t)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (int8_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (int8_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, uint16_t &result)
{
	if (convert<uint16_t>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (uint16_t)(char)(value);
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = (uint16_t)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (uint16_t)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (uint16_t)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (uint16_t)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (uint16_t)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (uint16_t)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (uint16_t)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (uint16_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (uint16_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, int16_t &result)
{
	if (convert<int16_t>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (int16_t)(char)(value);
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = (int16_t)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (int16_t)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (int16_t)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (int16_t)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (int16_t)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (int16_t)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (int16_t)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (int16_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (int16_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, uint32_t &result)
{
	if (convert<uint32_t>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (uint32_t)(char)(value);
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = (uint32_t)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (uint32_t)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (uint32_t)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (uint32_t)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (uint32_t)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (uint32_t)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (uint32_t)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (uint32_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (uint32_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, int32_t &result)
{
	if (convert<int32_t>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (int32_t)(char)(value);
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = (int32_t)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (int32_t)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (int32_t)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (int32_t)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (int32_t)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (int32_t)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (int32_t)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (int32_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (int32_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, uint64_t &result)
{
	if (convert<uint64_t>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (uint64_t)(char)(value);
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = (uint64_t)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (uint64_t)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (uint64_t)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (uint64_t)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (uint64_t)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (uint64_t)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (uint64_t)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (uint64_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (uint64_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, int64_t &result)
{
	if (convert<int64_t>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (int64_t)(char)(value);
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = (int64_t)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (int64_t)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (int64_t)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (int64_t)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (int64_t)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (int64_t)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (int64_t)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = (int64_t)(float)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (int64_t)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, float &result)
{
	if (convert<float>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = (float)(char)(value);
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = (float)(uint8_t)(value);
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = (float)(int8_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = (float)(uint16_t)(value);
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = (float)(int16_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = (float)(uint32_t)(value);
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = (float)(int32_t)(value);
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = (float)(uint64_t)(value);
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = (float)(int64_t)(value);
		return true;
	}
	else if (value.type() == typeid(double))
	{
		result = (float)(double)(value);
		return true;
	}

	// fail
	return false;
}

bool hvalue::convert(hvalue &value, double &result)
{
	if (convert<double>(value, result))
		return true;

	if (value.type() == typeid(char))
	{
		result = static_cast<char>((uint8_t)(value));
		return true;
	}
	else if (value.type() == typeid(uint8_t))
	{
		result = static_cast<double>((uint8_t)(value));
		return true;
	}
	else if (value.type() == typeid(int8_t))
	{
		result = static_cast<double>((int8_t)(value));
		return true;
	}
	else if (value.type() == typeid(uint16_t))
	{
		result = static_cast<double>((uint16_t)(value));
		return true;
	}
	else if (value.type() == typeid(int16_t))
	{
		result = static_cast<double>((int16_t)(value));
		return true;
	}
	else if (value.type() == typeid(uint32_t))
	{
		result = static_cast<double>((uint32_t)(value));
		return true;
	}
	else if (value.type() == typeid(int32_t))
	{
		result = static_cast<double>((int32_t)(value));
		return true;
	}
	else if (value.type() == typeid(uint64_t))
	{
		result = static_cast<double>((uint64_t)(value));
		return true;
	}
	else if (value.type() == typeid(int64_t))
	{
		result = static_cast<double>((int64_t)(value));
		return true;
	}
	else if (value.type() == typeid(float))
	{
		result = static_cast<double>((float)(value));
		return true;
	}

	// fail
	return false;
}

const std::type_info &hvalue::type() const { return content ? content->type() : typeid(undefined_t); }

bool hvalue::empty() const { return content == nullptr || content->type() == typeid(undefined_t); }

bool hvalue::isNil() const
{
	if (type() == typeid(std::nullptr_t))
		return true;
	else if (type() == typeid(HObject *))
		return static_cast<holder<HObject *> *>(content.get())->value != nullptr;

	return false;
}

/*
    template <typename T>
    object::operator T() const
    {
        if (content->type() == typeid(T))
            return static_cast<holder<T> *>(content.get())->value;
        else if (content->type() == typeid(T *))
            return *static_cast<holder<T *> *>(content.get())->value;
        else if (content->type() == typeid(Object *))
        {
            // compilation safety
            typedef typename std::conditional<std::is_polymorphic<T>::value, T, void> final_type;
            
            // runtime safety
            if (typeid(final_type *) == typeid(T))
            {
                Object *obj = static_cast<object::holder<Object *> *>(content.get())->value;
                
                // try polymorphic cast
                if (final_type *cast = dynamic_cast<final_type *>(obj))
                {
                    // success!
                    return *static_cast<object::holder<T *> *>(content.get())->value;
                }
            }
        }
        
        throw 1; // bad cast
        // fail
        //throw new TypeError("Cannot cast object"); // TODO new TypeCastError()
    }
    
    template <typename T>
    object::operator T *() const
    {
        if (content->type() == typeid(std::nullptr_t) || content->type() == typeid(undefined_t))
            return nullptr;
        else if (content->type() == typeid(T *))
            return static_cast<holder<T *> *>(content.get())->value;
        else if (content->type() == typeid(T))
            return &static_cast<holder<T> *>(content.get())->value;
        else if (content->type() == typeid(Object *))
        {
            Object *obj = static_cast<holder<Object *> *>(content.get())->value;
            if (T *cast = dynamic_cast<T *>(obj))
                return cast;
        }
        else if (typeid(T) == typeid(Object *)) //|| content->isClassOf<T>()
        {
            T *x;
            
            if (content->type() == typeid(Object *))
                x = static_cast<holder<T *> *>(content.get())->value;
                }
        
        // fail
        throw 1;
        //throw new TypeError("Cannot cast object"); // TODO new TypeCastError()
    }
    
    template <typename T>
    object::operator const T *() const
    {
        if (content->type() == typeid(T))
            return static_cast<holder<T *> *>(content.get())->value;
        else if (content->type() == typeid(T))
            return &static_cast<holder<T> *>(content.get())->value;
        else if (content->type() == typeid(Object *))
        {
            Object *obj = static_cast<holder<Object *> *>(content.get())->value;
            if (T *cast = dynamic_cast<T>(obj))
                return cast;
        }
        
        // fail
        throw 1;
        //throw new TypeError("Cannot cast object"); // TODO new TypeCastError()
    }*/
/*
     template <typename T>
     object::operator object_ptr<T>() const
     {
     object_ptr<T> ptr = nullptr;
     
     if (content->type() == typeid(Object *))
     {
     Object *obj = *this;
     if (T *value = dynamic_cast<T *>(obj))
     ptr = value; // success
     }
     
     return ptr;
     }
     */

hvalue &hvalue::operator=(const hvalue &o)
{
	if (&o != this)
	{
		release();
		if (o.content)
			content = o.content->clone();
		hold();
	}
	return (*this);
}

hvalue &hvalue::operator=(hvalue &&o)
{
	if (&o != this)
	{
		release();
		if (o.content)
			content = std::move(o.content);
		hold();
	}
	return (*this);
}

hvalue::operator bool() const
{
	if (content->type() == typeid(HObject *))
	{
		HObject *obj = *this;

		if (!obj)
			return false;
		else if (hstring str = tryCast<HString>())
		{
			std::string s = str->toString();
			return !s.empty();
		}

		return ((HObject *)*this) != nullptr;
	}
	else if (content->type() == typeid(bool))
		return static_cast<holder<bool> *>(content.get())->value;
	else if (content->type() == typeid(char))
		return (char)*this != '\0';
	else if (content->type() == typeid(uint8_t))
		return (uint8_t) * this != 0;
	else if (content->type() == typeid(int8_t))
		return (int8_t) * this != 0;
	else if (content->type() == typeid(uint16_t))
		return (uint16_t) * this != 0;
	else if (content->type() == typeid(int16_t))
		return (int16_t) * this != 0;
	else if (content->type() == typeid(uint32_t))
		return (uint32_t) * this != 0;
	else if (content->type() == typeid(int32_t))
		return (int32_t) * this != 0;
	else if (content->type() == typeid(uint64_t))
		return (uint64_t) * this != 0;
	else if (content->type() == typeid(int64_t))
		return (int64_t) * this != 0;
	else if (content->type() == typeid(float))
		return (float)*this != 0;
	else if (content->type() == typeid(double))
		return (double)*this != 0;

	return false;
}

hvalue hvalue::exp(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		hvalue::convert(lhs, a);
		hvalue::convert(rhs, b);
		return std::pow(a, b);
	}

	// fail
	return NaN;
}

bool hvalue::operator!() const
{
	if (content->type() == typeid(HObject *))
	{
		HObject *obj = *this;

		if (obj)
		{
			if (hstring str = tryCast<HString>())
				return str->toString().empty();

			return false;
		}

		return true;
	}
	else if (content->type() == typeid(bool))
		return !((bool)*this);
	else if (content->type() == typeid(char))
		return !((char)*this);
	else if (content->type() == typeid(uint8_t))
		return !((uint8_t) * this);
	else if (content->type() == typeid(int8_t))
		return !((int8_t) * this);
	else if (content->type() == typeid(uint16_t))
		return !((uint16_t) * this);
	else if (content->type() == typeid(int16_t))
		return !((int16_t) * this);
	else if (content->type() == typeid(uint32_t))
		return !((uint32_t) * this);
	else if (content->type() == typeid(int32_t))
		return !((int32_t) * this);
	else if (content->type() == typeid(uint64_t))
		return !((uint64_t) * this);
	else if (content->type() == typeid(int64_t))
		return !((int64_t) * this);
	else if (content->type() == typeid(float))
		return !((float)*this);
	else if (content->type() == typeid(double))
		return !((double)*this);

	return false;
}

bool hvalue::operator==(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (content->type() == rhs.type() && (content->type() == typeid(std::nullptr_t) || content->type() == typeid(undefined_t)))
		return true;

	if (content->type() == typeid(bool) && content->type() == rhs.type())
	{
		bool a = convert(lhs, a);
		bool b = convert(rhs, b);
		return a == b;
	}
	else if (content->type() == typeid(char16_t) && content->type() == rhs.type())
	{
		char16_t a;
		char16_t b;
		convert(lhs, a);
		convert(rhs, b);
		return a != b;
	}
	else if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a == b;
	}
    else if(is_object(lhs))
    {
        HObject *obj = lhs; // cast
        return obj->equals(rhs);
    }

	// fail
	return false;
}

bool hvalue::operator!=(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (content->type() == rhs.type() && (type() == typeid(std::nullptr_t) || type() == typeid(undefined_t)))
		return false;

	if (content->type() == typeid(bool) && content->type() == rhs.type())
	{
		bool a;
		bool b;
		convert(lhs, a);
		convert(rhs, b);
		return a != b;
	}
	else if (content->type() == typeid(char16_t) && content->type() == rhs.type())
	{
		char16_t a;
		char16_t b;
		convert(lhs, a);
		convert(rhs, b);
		return a != b;
	}
	else if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a != b;
	}
    else if(is_object(lhs))
    {
        HObject *obj = lhs; // cast
        return !obj->equals(rhs);
    }
    
	return true;
}

hvalue hvalue::operator&&(hvalue &rhs) const
{

	return false;
}

hvalue hvalue::operator||(hvalue &rhs) const
{

	return false;
}

hvalue hvalue::operator>(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a > b;
	}

	// fail
	return false;
}

hvalue hvalue::operator>=(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a >= b;
	}

	// fail
	return false;
}

hvalue hvalue::operator<(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a < b;
	}

	// fail
	return false;
}

hvalue hvalue::operator<=(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a <= b;
	}

	// fail
	return false;
}

hvalue hvalue::operator+() const
{
	hvalue value = (*this);

	if (is_number(value))
	{
		double a;
		convert(value, a);
		return +a;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator-() const
{
	hvalue value = (*this);

	if (is_number(value))
	{
		double a;
		convert(value, a);
		return -a;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator*(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a * b;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator/(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		if (b == 0)
			return undefined; // division by zero
		return a / b;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator%(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return std::fmod(a, b);
	}

	// fail
	return NaN;
}

hvalue hvalue::operator+(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a + b; // addition
	}

	// fail
	return NaN;
}

hvalue hvalue::operator-(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		double a;
		double b;
		convert(lhs, a);
		convert(rhs, b);
		return a - b;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator&(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		uint32_t a;
		uint32_t b;
		convert(lhs, a);
		convert(rhs, b);
		return a & b;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator|(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		uint32_t a;
		uint32_t b;
		convert(lhs, a);
		convert(rhs, b);
		return a | b;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator^(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		uint32_t a;
		uint32_t b;
		convert(lhs, a);
		convert(rhs, b);
		return a ^ b;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator~() const
{
	hvalue value = (*this);

	if (is_number(value))
	{
		int32_t a;
		convert(value, a);
		return ~a;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator>>(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		uint32_t a;
		uint32_t b;
		convert(lhs, a);
		convert(rhs, b);
		return a >> b;
	}

	// fail
	return NaN;
}

hvalue hvalue::operator<<(hvalue &rhs) const
{
	hvalue lhs = (*this);

	if (is_number(lhs) && is_number(rhs))
	{
		uint32_t a;
		uint32_t b;
		convert(lhs, a);
		convert(rhs, b);
		return a << b;
	}

	return NaN;
}

hvalue::operator std::string() const
{
	std::string s;
	hvalue o = (*this);
	convert(o, s);
	return s;
}

bool is_undefined(const hvalue &o)
{
	return o.type() == typeid(undefined_t);
}

bool is_null(const hvalue &o)
{
	if (o.type() == typeid(HObject *))
		return ((HObject *)o) == nullptr;

	return o.type() == typeid(std::nullptr_t);
}

bool is_byte(const hvalue &o)
{
	return o.type() == typeid(uint8_t);
}

bool is_sbyte(const hvalue &o)
{
	return o.type() == typeid(int8_t);
}

bool is_ushort(const hvalue &o)
{
	return o.type() == typeid(uint16_t);
}

bool is_short(const hvalue &o)
{
	return o.type() == typeid(int16_t);
}

bool is_uint(const hvalue &o)
{
	return o.type() == typeid(uint32_t);
}

bool is_int(const hvalue &o)
{
	return o.type() == typeid(int32_t);
}

bool is_ulong(const hvalue &o)
{
	return o.type() == typeid(uint64_t);
}

bool is_long(const hvalue &o)
{
	return o.type() == typeid(int64_t);
}

bool is_float(const hvalue &o)
{
	return o.type() == typeid(float);
}

bool is_double(const hvalue &o)
{
	return o.type() == typeid(double);
}

bool is_char(const hvalue &o)
{
	return o.type() == typeid(char);
}

bool is_bool(const hvalue &o)
{
	return o.type() == typeid(bool);
}

bool is_number(const hvalue &o)
{
	return is_byte(o) || is_sbyte(o) || is_ushort(o) || is_short(o) || is_uint(o) || is_int(o) || is_ulong(o) || is_long(o) || is_float(o) || is_double(o) || is_char(o);
}

bool is_primitive(const hvalue &o)
{
	return is_number(o) || is_bool(o);
}

hobject is_object(const hvalue &o)
{
    return o.tryCast<HObject>();
}

hstring is_string(const hvalue &o)
{
    return o.tryCast<HString>();
}

hlist is_list(const hvalue &o)
{
    return o.tryCast<HList>();
}

bool is_const_function(const hvalue &o)
{
    return o.type() == typeid(FuncData *);
}

bool is_const_class(const hvalue &o)
{
    return o.type() == typeid(ClassData *);
}

bool is_callable(const hvalue &o)
{
    if(is_object(o))
    {
        HObject *obj = o;
        if(dynamic_cast<HvmCallable *>(obj))
            return true; // callable
    }
    
    return false;
}

hfunction is_function(const hvalue &o)
{
	return o.tryCast<HFunction>();
}

hclass is_class(const hvalue &o)
{
    return o.tryCast<HClass>();
}

hconstructor is_constructor(const hvalue &o)
{
    return o.tryCast<HConstructor>();
}

hproperty is_property(const hvalue &o)
{
    return o.tryCast<HProperty>();
}

hgetter is_getter(const hvalue &o)
{
    return o.tryCast<HGetter>();
}

hsetter is_setter(const hvalue &o)
{
    return o.tryCast<HSetter>();
}

hmethod is_method(const hvalue &o)
{
    return o.tryCast<HMethod>();
}

haction is_action(const hvalue &o)
{
    return o.tryCast<HAction>();
}

hcontext is_context(const hvalue &o)
{
    return o.tryCast<HContext>();
}

hevent is_event(const hvalue &o)
{
    return o.tryCast<HEventContext>();
}

hetype is_etype(const hvalue &o)
{
    return o.tryCast<HEventType>();
}

hdate is_date(const hvalue &o)
{
    return o.tryCast<HDate>();
}

herror is_error(const hvalue &o)
{
    return o.tryCast<HError>();
}

bool is_env(const hvalue &o)
{
	return o.type() == typeid(HvmEnv *);
}

} // namespace hydro
