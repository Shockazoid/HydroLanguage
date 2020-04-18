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

#ifndef __h3o_hvm_ref_cast__
#define __h3o_hvm_ref_cast__

#include <typeinfo>
#include <type_traits>

#include "hvalue.hpp"
#include "../memory/object_ptr.hpp"

namespace hydro
{

template <typename T>
auto fetch_back(T &t) -> typename std::remove_reference<decltype(t.back())>::type
{
	typename std::remove_reference<decltype(t.back())>::type ret = t.back();
	t.pop_back();
	return ret;
}

template <typename T>
struct hvm_ref_cast
{
	T do_cast(hvalue o)
	{
		return (T)o;
	}
};

template <typename T>
struct hvm_ref_cast<object_ptr<T> &>
{
	object_ptr<T> do_cast(hvalue o)
	{
		object_ptr<T> ptr = (T *)o;
		return ptr;
	}
};

} // namespace hydro

#endif /* __h3o_hvm_ref_cast__ */
