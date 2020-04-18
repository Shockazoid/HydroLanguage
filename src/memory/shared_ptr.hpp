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

#ifndef __h3o_shared_ptr__
#define __h3o_shared_ptr__

#include <cstdlib>
#include <cstddef>
#include <type_traits>

namespace hydro
{

struct shared_counter
{
	uint32_t value{0};
	void operator++(int)
	{
		++value;
	}
	void operator--(int)
	{
		if (value != 0)
			--value;
	}
};

template <typename T>
class shared_ptr
{
	template <typename X>
	friend class shared_ptr;

private:
	T *_subject{nullptr};
	shared_counter *_counter{nullptr};

	void hold()
	{
		if (_counter && _subject)
			(*_counter)++;
	}

	void release()
	{
		if (_counter && _subject)
		{
			//if(_counter->value == 0)
			//        std::cerr << "reference deleted already" << std::endl;

			(*_counter)--;

			if (_counter->value == 0)
			{
				delete _counter;
				delete _subject;
			}

			_counter = nullptr;
			_subject = nullptr;
		}
	}

public:
	// default constructor
	shared_ptr() {}

	// value constructor
	shared_ptr(T *subject) : _subject{subject}
	{
		if (subject)
			_counter = new shared_counter{};
		hold();
	}

	// copy constructor
	template <typename X>
	shared_ptr(const shared_ptr<X> &ptr) : _subject{ptr._subject}, _counter{ptr._counter}
	{
		hold();
	}
	/*
	// move constructor
	template <typename X>
	shared_ptr(shared_ptr<X> &&ptr) : _subject{std::move(ptr._subject)}, _counter{std::move(ptr._counter)}
	{
		hold();
	}
    */
	// destructor
	virtual ~shared_ptr()
	{
		release();
	}

	// helpers
	bool empty() const { return _subject == nullptr; }
	T *get() const { return _subject; }
	template <typename X>
	shared_ptr<X> tryCast() const
	{
		shared_ptr<X> cpy; // copy

		if (X *x = dynamic_cast<X *>(_subject))
		{
			cpy._subject = x;
			cpy._counter = _counter;
			cpy.hold();
		}

		return cpy;
	}

	// copy assignment
	shared_ptr<T> &operator=(const shared_ptr<T> &rhs)
	{
		//if (this != &rhs)
		if (_subject != rhs._subject)
		{
			release();
			_counter = rhs._counter;
			_subject = rhs._subject;
			hold();
		}

		return *this;
	}
	/*
	// move assignment
	template <typename X>
	shared_ptr<T> &operator=(shared_ptr<X> &&rhs)
	{
		if (_subject != rhs._subject)
		{
			release();
			_counter = std::move(rhs._counter);
			_subject = std::move(rhs._subject);
			hold();
		}

		return *this;
	}*/

	operator bool() const { return _subject != nullptr; }
	bool operator!() const { return _subject == nullptr; }

	//
	bool operator==(std::nullptr_t) const { return _subject == nullptr; }
	template <typename X>
	bool operator==(X *x) const { return _subject == x; }
	template <typename X>
	bool operator==(const shared_ptr<X> &ptr) const { return _subject == ptr._subject; }
	bool operator!=(std::nullptr_t) const { return _subject != nullptr; }
	template <typename X>
	bool operator!=(X *x) const { return _subject != x; }
	template <typename X>
	bool operator!=(const shared_ptr<X> &ptr) const { return _subject != ptr._subject; }
	T &operator*() const { return *_subject; }
	T *operator->() const { return _subject; }
};

} // namespace hydro

#endif /* __h3o_shared_ptr__ */
