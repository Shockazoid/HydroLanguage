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

#ifndef __h3o_hvalue__
#define __h3o_hvalue__

#include <type_traits>
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <cmath>
#include <list>

#include "../utility/any.hpp"

namespace hydro
{

class HObject;
class HContext;

template <typename X>
class object_ptr;

struct undefined_t final
{
    undefined_t() {}
};

const undefined_t undefined{};

const double NaN = std::nan("");

const double Infinity = std::numeric_limits<double>::infinity();

class hvalue final
{
	friend class VM;
	friend class HvmEnv;
    /*template <typename X>
    friend void hvm_auto_wire(std::list<object> &, struct AutoWireContext *, uint16_t &);
    template <typename X>
    friend void hvm_auto_wire(object, std::list<object> &, struct AutoWireContext *, uint16_t &);*/
    template <typename X>
    friend struct hvm_auto_wire;

private:
	struct placeholder
	{
		virtual ~placeholder() {}
		virtual const std::type_info &type() const = 0;
		virtual std::unique_ptr<placeholder> clone() const = 0;
		virtual any castAny() const = 0;
	};

	template <typename T>
	struct holder final : public placeholder
	{
		T value;
		holder(T x) : value{x} {}
		virtual ~holder() {}
		virtual const std::type_info &type() const override { return typeid(T); }
		virtual std::unique_ptr<placeholder> clone() const override { return std::unique_ptr<placeholder>{new holder<T>(value)}; }
		virtual any castAny() const override { return value; }
	};

	std::unique_ptr<placeholder> content;

	template <typename T>
	static bool convert(hvalue &value, T &result);
	static bool convert(hvalue &value, bool &result); // native bool
	static bool convert(hvalue &value, char &result);
	static bool convert(hvalue &value, char16_t &result);
	static bool convert(hvalue &value, const char *&result); // native string
	static bool convert(hvalue &value, std::string &result); // native string
	static bool convert(hvalue &value, uint8_t &result);     // native byte
	static bool convert(hvalue &value, int8_t &result);	// native sbyte
	static bool convert(hvalue &value, uint16_t &result);    // native ushort
	static bool convert(hvalue &value, int16_t &result);     // native short
	static bool convert(hvalue &value, uint32_t &result);    // native uint
	static bool convert(hvalue &value, int32_t &result);     // native int
	static bool convert(hvalue &value, uint64_t &result);    // native ulong
	static bool convert(hvalue &value, int64_t &result);     // native long
	static bool convert(hvalue &value, float &result);	 // native float
	static bool convert(hvalue &value, double &result);	// native double

	void hold() const;
	void release() const;

	template <typename X>
	explicit hvalue(X *x) : content{new holder<X *>{x}} {}

public:
	hvalue();				// default
	hvalue(undefined_t o);		// undefined
	hvalue(std::nullptr_t o);     // null
	hvalue(const hvalue &o);	// copy
	hvalue(hvalue &&o);		// move
	hvalue(uint8_t o);		// byte
	hvalue(int8_t o);			// sbyte
	hvalue(uint16_t o);		// ushort
	hvalue(int16_t o);		// short
	hvalue(uint32_t o);		// uint
	hvalue(int32_t o);		// int
	hvalue(uint64_t o);		// ulong
	hvalue(int64_t o);		// long
	hvalue(float o);			// float
	hvalue(double o);			// double
	hvalue(char o);			// char
	hvalue(char16_t o);		// char
	hvalue(bool o);			// bool
	hvalue(std::string o);		// string
    hvalue(const char *o);      // c string
	hvalue(HObject *o);		// Object
	~hvalue();

	template <typename T>
	static bool equals(hvalue &a, hvalue &b);

	const std::type_info &type() const;
	bool empty() const;
	bool isNil() const;

	template <typename T>
	object_ptr<T> tryCast() const
	{
		if (std::is_base_of<HObject, T>() && content->type() == typeid(HObject *))
		{
			// try cast
			return dynamic_cast<T *>(static_cast<holder<HObject *> *>(content.get())->value);
		}

		// fail
		return nullptr;
	}

	// casts

	// implicit casts
	template <typename T>
	operator T *() const
	{
		if (content->type() == typeid(std::nullptr_t) || content->type() == typeid(undefined_t))
			return nullptr;
		else if (content->type() == typeid(T *))
			return static_cast<holder<T *> *>(content.get())->value;
		else if (content->type() == typeid(T))
			return &static_cast<holder<T> *>(content.get())->value;
		else if (content->type() == typeid(HObject *))
		{
			HObject *obj = static_cast<holder<HObject *> *>(content.get())->value;
			if (T *cast = dynamic_cast<T *>(obj))
				return cast;
		}
		else if (typeid(T) == typeid(HObject *)) //|| content->isClassOf<T>()
		{
			T *x;

			if (content->type() == typeid(HObject *))
				x = static_cast<holder<T *> *>(content.get())->value;
		}

		// fail
		throw std::runtime_error{"Bad cast: " + std::string{content->type().name()} + " to " + std::string{typeid(T *).name()}};
		//throw new TypeError("Cannot cast object"); // TODO new TypeCastError()
	}

	template <typename T>
	operator T() const
	{
		if (content->type() == typeid(T))
			return static_cast<holder<T> *>(content.get())->value;
		else if (content->type() == typeid(T *))
			return *static_cast<holder<T *> *>(content.get())->value;
		else if (content->type() == typeid(HObject *))
		{
			// compilation safety
			typedef typename std::conditional<std::is_polymorphic<T>::value, T, void> final_type;

			// runtime safety
			if (typeid(final_type *) == typeid(T))
			{
				HObject *obj = static_cast<hvalue::holder<HObject *> *>(content.get())->value;

				// try polymorphic cast
				if (final_type *cast = dynamic_cast<final_type *>(obj))
				{
					// success!
					return *static_cast<hvalue::holder<T *> *>(content.get())->value;
				}
			}
		}
        else
        {
            // try implicit conversion
            if (typeid(T) == typeid(uint32_t) ||
                typeid(T) == typeid(uint16_t) ||
                typeid(T) == typeid(int16_t) ||
                typeid(T) == typeid(uint64_t) ||
                typeid(T) == typeid(int64_t) ||
                typeid(T) == typeid(char) ||
                typeid(T) == typeid(char16_t) ||
                typeid(T) == typeid(float) ||
                typeid(T) == typeid(bool) ||
                typeid(T) == typeid(int32_t) ||
                typeid(T) == typeid(double)
               )
            {
                T result;
                hvalue subject = *this;
                if(convert(subject, result))
                    return result;
            }
        }
        
		throw std::runtime_error{type().name()};
		//throw new std::runtime_error("Cannot cast object of type " + get_type_name(this) + " to object of type " + get_type_name<T>()); // TODO new TypeCastError()
	}
	template <typename T>
	operator const T *() const
	{
		if (content->type() == typeid(T))
			return static_cast<holder<T *> *>(content.get())->value;
		else if (content->type() == typeid(T))
			return &static_cast<holder<T> *>(content.get())->value;
		else if (content->type() == typeid(HObject *))
		{
			HObject *obj = static_cast<holder<HObject *> *>(content.get())->value;
			if (T *cast = dynamic_cast<T>(obj))
				return cast;
		}

		// fail
		throw 1;
		//throw new TypeError("Cannot cast object"); // TODO new TypeCastError()
	}

	template <typename T>
	operator object_ptr<T>() const
	{
		if (content->type() == typeid(HObject *))
		{
			HObject *obj = *this; // cast
			if (obj)
			{
				object_ptr<T> cast = obj;
				return cast;
			}
		}

		throw 1;
	}
    
    template <typename T>
    operator const T*()
    {
        if(content->type() == typeid(T *))
            return  static_cast<holder<T *> *>(content.get())->value;
        
        throw 1;
    }
    
	// assignment
	hvalue &operator=(hvalue &&rhs);
	hvalue &operator=(const hvalue &rhs);

	operator bool() const;

	// operators
	hvalue exp(hvalue &rhs) const;	// exponential
	bool operator!() const;			// logical NOT
	bool operator==(hvalue &rhs) const; // equality
	bool operator!=(hvalue &rhs) const; // inequality
	hvalue operator&&(hvalue &rhs) const;
	hvalue operator||(hvalue &rhs) const;
	hvalue operator>(hvalue &rhs) const;  // greater than
	hvalue operator>=(hvalue &rhs) const; // greater than or equal
	hvalue operator<(hvalue &rhs) const;  // less than
	hvalue operator<=(hvalue &rhs) const; // less than or equal
	hvalue operator+() const;		  // positive
	hvalue operator-() const;		  // negation
	hvalue operator*(hvalue &rhs) const;  // multiplication
	hvalue operator/(hvalue &rhs) const;  // divisionconst
	hvalue operator%(hvalue &rhs) const;  // modulo
	hvalue operator+(hvalue &rhs) const;  // addition
	hvalue operator-(hvalue &rhs) const;  // subtraction
	hvalue operator&(hvalue &rhs) const;  // bitwise AND
	hvalue operator|(hvalue &rhs) const;  // bitwise OR
	hvalue operator^(hvalue &rhs) const;  // bitwise XOR
	hvalue operator~() const;		  // bitwise NOT
	hvalue operator>>(hvalue &rhs) const; // bitewise right shift
	hvalue operator<<(hvalue &rhs) const; // bitewise left shift
	operator std::string() const;
	operator any() const { return content->castAny(); }
};

bool is_undefined(const hvalue &o);

bool is_null(const hvalue &o);

bool is_byte(const hvalue &o);

bool is_sbyte(const hvalue &o);

bool is_ushort(const hvalue &o);

bool is_short(const hvalue &o);

bool is_uint(const hvalue &o);

bool is_int(const hvalue &o);

bool is_ulong(const hvalue &o);

bool is_long(const hvalue &o);

bool is_float(const hvalue &o);

bool is_double(const hvalue &o);

bool is_char(const hvalue &o);

bool is_bool(const hvalue &o);

bool is_number(const hvalue &o);

bool is_primitive(const hvalue &o);

bool is_const_function(const hvalue &o);

bool is_const_class(const hvalue &o);

bool is_callable(const hvalue &o);

object_ptr<HObject> is_object(const hvalue &o);

object_ptr<class HString> is_string(const hvalue &o);

object_ptr<class HList> is_list(const hvalue &o);

object_ptr<class HFunction> is_function(const hvalue &o);

object_ptr<class HClass> is_class(const hvalue &o);

object_ptr<class HConstructor> is_constructor(const hvalue &o);

object_ptr<class HProperty> is_property(const hvalue &o);

object_ptr<class HGetter> is_getter(const hvalue &o);

object_ptr<class HSetter> is_setter(const hvalue &o);

object_ptr<class HMethod> is_method(const hvalue &o);

object_ptr<class HAction> is_action(const hvalue &o);

object_ptr<class HContext> is_context(const hvalue &o);

object_ptr<class HEventContext> is_event(const hvalue &o);

object_ptr<class HEventType> is_etype(const hvalue &o);

object_ptr<class HDate> is_date(const hvalue &o);

object_ptr<class HError> is_error(const hvalue &o);

bool is_env(const hvalue &o);

template <typename T>
static constexpr bool valid_hvm_type()
{
	return std::is_same<void, T>() ||
		 std::is_same<undefined_t, T>() ||
		 std::is_same<std::nullptr_t, T>() ||
		 std::is_same<bool, T>() ||
		 std::is_same<char, T>() ||
		 std::is_same<char16_t, T>() ||
		 std::is_same<wchar_t, T>() ||
		 std::is_same<uint8_t, T>() ||
		 std::is_same<int8_t, T>() ||
		 std::is_same<uint16_t, T>() ||
		 std::is_same<int16_t, T>() ||
		 std::is_same<uint32_t, T>() ||
		 std::is_same<int32_t, T>() ||
		 std::is_same<uint64_t, T>() ||
		 std::is_same<int64_t, T>() ||
		 std::is_same<float, T>() ||
		 std::is_same<double, T>() ||
		 std::is_same<hvalue, T>() ||
		 std::is_convertible<object_ptr<HObject>, T>();
}

template <typename T>
static constexpr void __assert_hvm_arg()
{
	static_assert(valid_hvm_type<T>(), "Invalid argument type on supplied delegate.");
}

template <typename... Arg>
static constexpr void assert_hvm_args()
{
	int32_t _[] = {0, (__assert_hvm_arg<Arg>(), 0)...};
	static_assert(_);
}

// macro is used to assert that
#define hvm_assert_type(type, error_text) \
	static_assert(valid_hvm_type<type>(), error_text);

#define hvm_assert_args(...) \
	static_assert(assert_hvm_args<__VA_ARGS__>(), "Encountered invalid argument type on supplied delegate.")

template <typename ReturnType, typename... Arg>
static constexpr void check_delegate(ReturnType (*fn)(Arg...))
{
	// assert return type
	hvm_assert_type(ReturnType, "Encountered invalid return type on supplied delegate.");

	// assert argument type
	assert_hvm_args<Arg...>();
}

#define hvm_assert_delegate(fn) \
	check_delegate(fn)

} // namespace hydro

#endif /* __h3o_hvalue__ */
