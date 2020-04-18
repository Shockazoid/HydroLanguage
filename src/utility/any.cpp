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

#include "any.hpp"

namespace hydro
{

any::any() : _content{nullptr} {}

any::any(any &&x) : _content{std::move(x._content)} {}

any::any(any &x)
{
	if (x._content)
		_content = x._content->clone();
}

any::any(const any &x)
{
	if (x._content)
		_content = x._content->clone();
}

any::~any() {}

any &any::operator=(any &&rhs)
{
	_content = std::move(rhs._content);
	return (*this);
}

any &any::operator=(const any &rhs)
{
	_content = std::move(any(rhs)._content);
	return (*this);
}

bool any::empty() const { return _content == nullptr; }

const std::type_info &any::type() const { return _content ? _content->type() : typeid(void); }

void any::clear() { _content.reset(nullptr); }

} // namespace hydro
