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

#ifndef __h3o_object_ptr__
#define __h3o_object_ptr__

#include <type_traits>

#include "../vm/hvalue.hpp"

namespace hydro
{

template <typename T>
class object_ptr
{
	//static_assert(std::is_base_of<Object, T>(), "Template argument must point to a hydro::Object type.");
	//static_assert(!std::is_pointer<T>() && !std::is_reference<T>(), "Template argument not be a pointer or a reference.");

	friend class hvalue;
	template <typename X>
	friend class object_ptr;

private:
	T *_subject;

public:
	object_ptr() { _subject = nullptr; }
	object_ptr(std::nullptr_t) { _subject = nullptr; }
	template <typename X>
	object_ptr(const object_ptr<X> &ptr)
	{
		//static_assert(std::is_base_of<T, X>(), "Instance is not valid with type.");

		X *x = ptr._subject;
		_subject = x;

		if (_subject)
			_subject->addRef();
	}
	object_ptr(hvalue x)
	{
		HObject *o = x;
		if (T *cast = dynamic_cast<T *>(o))
		{
			_subject = cast;
			_subject->addRef();
		}
	}
	template <typename X>
	object_ptr(X *x)
	{
		//static_assert(std::is_same<T, X>() || std::is_base_of<T, X>(), "Value must be a subclass of template argument.");

		_subject = static_cast<T *>(x);
		if (_subject)
			_subject->addRef();
	}
	virtual ~object_ptr()
	{
		if (_subject)
		{
			_subject->removeRef();
			_subject = nullptr;
		}
	}
	T *get() const { return _subject; }
	bool empty() const { return _subject == nullptr; }
	template <typename X>
	object_ptr<X> tryCast() const
	{
		//static_assert(std::is_base_of<Object, X>(), "Template argument must be a subclass of the Object class.");

		// try cast
		X *x = dynamic_cast<X *>(_subject);
		object_ptr<X> ptr = x;
		return x;
	}
	operator bool() const { return _subject != nullptr; }
	bool operator!() const { return _subject == nullptr; }
	operator hvalue() const { return (HObject *)_subject; }
	object_ptr<T> operator=(std::nullptr_t rhs)
	{
		// nullify
		if (_subject != nullptr)
		{
			_subject->removeRef();
			_subject = rhs;
		}

		return *this;
	}
	object_ptr<T> operator=(hvalue &rhs)
	{
		// nullify
		if (_subject != nullptr)
		{
			_subject->removeRef();
			_subject = nullptr;
		}

		return *this;
	}
	template <typename X>
	object_ptr<X> operator=(object_ptr<X> &rhs)
	{
		//static_assert(std::is_same<Object, X>() || std::is_base_of<Object, X>(), "Template argument must point to a hydro::Object type.");
		//static_assert(!std::is_pointer<X>() && !std::is_reference<X>(), "Template argument not be a pointer or a reference.");

		if (_subject != rhs._subject)
		{
			if (_subject)
			{
				_subject->removeRef();
				_subject = nullptr;
			}

			// copy
			if (rhs._subject)
			{
				_subject = static_cast<T *>(rhs._subject);
				rhs._subject = nullptr;
				_subject->addRef();
			}
			else
				_subject = nullptr;
		}

		return *this;
	}
	template <typename X>
	object_ptr<T> operator=(X &rhs)
	{
		//static_assert(std::is_same<Object, X>() || std::is_base_of<Object, X>(), "Template argument must point to a hydro::Object type.");
		//static_assert(!std::is_pointer<X>() && !std::is_reference<X>(), "Template argument not be a pointer or a reference.");

		X *ptr = &rhs;

		if (_subject != ptr)
		{
			//Object *tmp = (Object *)ptr;

			if (ptr)
				ptr->addRef();

			if (_subject)
			{
				_subject->removeRef();
				_subject = nullptr;
			}

			_subject = ptr;
		}

		return *this;
	}

	object_ptr<T> operator=(T *rhs)
	{
		if (_subject != rhs)
		{
			T *tmp = (T *)rhs;

			if (tmp != nullptr)
				tmp->addRef();

			if (_subject)
			{
				_subject->removeRef();
				_subject = nullptr;
			}

			_subject = tmp;
		}

		return *this;
	}
	T &operator*() const { return *_subject; }
	T *operator->() const { return _subject; }
	operator T *() const { return (T *)_subject; }
	template <typename X>
	operator X *() const
	{
		//static_assert(std::is_same<Object, X>() || std::is_base_of<Object, X>(), "Template argument must point to a hydro::Object type.");
		//static_assert(!std::is_pointer<X>() && !std::is_reference<X>(), "Template argument not be a pointer or a reference.");

		return dynamic_cast<X *>(_subject);
	}
};

} // namespace hydro

#endif /* __h3o_object_ptr__ */
