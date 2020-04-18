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

#ifndef __h3o_CallService__
#define __h3o_CallService__

#include <list>

#include "RuntimeService.hpp"
#include "../hvalue.hpp"
#include "../../system/HObject.hpp"

namespace hydro
{

struct CallService : RuntimeService
{
    CallService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~CallService() {}
    
    hvalue target(hvalue callable, std::list<hvalue> &args, hvalue thisObject = nullptr) const;
    
    template <typename ...Args>
    hvalue target(hvalue callable, Args...args) const
    {
        std::list<hvalue> argList = { args... };
        return target(callable, argList);
    }
    
    hvalue global(std::string name, std::list<hvalue> &args) const;
    template <typename ...Args>
    hvalue global(std::string name, Args...args) const
    {
        std::list<hvalue> argList = { args... };
        return global(name, argList);
    }
    
    hvalue property(hobject instance, std::string name, std::list<hvalue> &args) const;
    template <typename ...Args>
    hvalue property(hobject instance, std::string name, Args...args) const
    {
        std::list<hvalue> argList = { args... };
        return property(instance, name, argList);
    }
    
    hvalue index(hobject instance, hvalue key, std::list<hvalue> &args) const;
    template <typename ...Args>
    hvalue index(hobject instance, hvalue key, Args...args) const
    {
        std::list<hvalue> argList = { args... };
        return index(instance, key, argList);
    }
};

} // namespace hydro

#endif /* __h3o_CallService__ */
