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

#ifndef __h3o_any__
#define __h3o_any__

#include <memory>

namespace hydro
{

class any;

template <typename T>
T any_cast(any &);
template <typename T>
T any_cast(const any &);
template <typename T>
T *any_cast(any *);
template <typename T>
const T *any_cast(const any *);

struct bad_any_cast : public std::bad_cast
{
};

class any final
{

	template <typename T>
	friend T any_cast(any &);
	template <typename T>
	friend T any_cast(const any &);
	template <typename T>
	friend T *any_cast(any *);
	template <typename T>
	friend const T *any_cast(const any *);

public:
	any();
	any(any &&x);
	any(any &x);
	any(const any &x);

	template <typename T>
	any(const T x) : _content{new holder<typename std::decay<const T>::type>(x)} {}

	~any();
	bool empty() const;
	const std::type_info &type() const;
	any &operator=(any &&rhs);
	any &operator=(const any &rhs);
	template <typename T>
	any &operator=(T &&x)
	{
		_content.reset(new holder<typename std::decay<T>::type>(typename std::decay<T>::type(x)));
		return (*this);
	}

	template <typename T>
	any &operator=(const T &x)
	{
		_content.reset(new holder<typename std::decay<T>::type>(typename std::decay<T>::type(x)));
		return (*this);
	}
	void clear();

private:
	struct placeholder
	{
		virtual std::unique_ptr<placeholder> clone() const = 0;
		virtual const std::type_info &type() const = 0;
		virtual ~placeholder() {}
	};

	template <typename T>
	struct holder : public placeholder
	{

		//holder(T &&x) : value{std::move(x)} {}
		holder(T x) : value{x} {}
		virtual ~holder() {}
		virtual std::unique_ptr<placeholder> clone() const override { return std::unique_ptr<placeholder>(new holder<T>(value)); }

		virtual const std::type_info &type() const override { return typeid(T); }

		T value;
	};

	void hold();
	void release();

	std::unique_ptr<placeholder> _content;
};

template <typename T>
T any_cast(any &x)
{
	if (x._content->type() != typeid(T))
		throw bad_any_cast();

	return static_cast<any::holder<T> *>(x._content.get())->value;
}

template <typename T>
T any_cast(const any &x)
{
	return any_cast<T>(any(x));
}

template <typename T>
T *any_cast(any *x)
{
	return dynamic_cast<T *>(x->_content.get());
}

template <typename T>
const T *any_cast(const any *x)
{
	return dynamic_cast<const T *>(x->_content.get());
}

} // namespace hydro

#endif /* __h3o_any__ */
