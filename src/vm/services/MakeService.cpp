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

#include "MakeService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

hobject MakeService::create(hclass type) const
{
    std::list<hvalue> args;
    return create(type, args);
}

hobject MakeService::create(hclass type, std::list<hvalue> &args) const
{
    return _vm->newInstance(_env->thread()->current(), type, args);
}

hobject MakeService::object(hclass type, std::list<hvalue> &args) const
{
    return create(type, args);
}

hobject MakeService::object() const { return create(_env->core()->ObjectClass()); }

hstring MakeService::string(std::string value) const { return new HString{_env, _env->core()->StringClass(), value}; }

hlist MakeService::list() const { return create(_env->core()->ListClass()).tryCast<HList>(); }

hjson MakeService::json() const { return create(_env->core()->JsonClass()).tryCast<HJson>(); }

hjson MakeService::json(std::string value) const
{
    std::list<hvalue> args;
    args.push_front(value);
    return create(_env->core()->JsonClass(), args).tryCast<HJson>();
}

hdictionary MakeService::dictionary() const { return create(_env->core()->DictionaryClass()).tryCast<HDictionary>(); }

hregex MakeService::regex(std::string pattern) const
{
    std::list<hvalue> args;
    args.push_front(pattern);
    return create(_env->core()->RegexClass(), args).tryCast<HRegex>();
}

} // namespace hydro
