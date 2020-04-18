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

#ifndef __h3o_InstanceMaker__
#define __h3o_InstanceMaker__

#include <list>

#include "RuntimeService.hpp"
#include "ThreadService.hpp"
#include "../hvalue.hpp"
#include "../../system/HObject.hpp"
#include "../../system/HClass.hpp"
#include "../../system/HString.hpp"
#include "../../system/HList.hpp"
#include "../../system/HDictionary.hpp"
#include "../../system/HJson.hpp"
#include "../../system/HXml.hpp"
#include "../../system/HRegex.hpp"

namespace hydro
{

struct MakeService : RuntimeService
{
private:
    hobject create(hclass type) const;
    hobject create(hclass type, std::list<hvalue> &args) const;
    
public:
    MakeService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~MakeService() {}
    hobject object(hclass type, std::list<hvalue> &args) const;
    template <typename ...Args>
    hobject object(hclass type, Args...args) const
    {
        std::list<hvalue> constructorArgs = { args... };
        return create(type, constructorArgs);
    }
    hobject object() const;
    hstring string(std::string value) const;
    hlist list() const;
    template <typename ...T>
    hlist list(T ...elements) const
    {
        hlist l = this->list();
        std::list<hvalue> args = { elements... };
        for(auto t : args)
            l->push(t);
        return l;
    }
    hjson json() const;
    hjson json(std::string jsonString) const;
    hdictionary dictionary() const;
    hregex regex(std::string pattern) const;
};

} // namespace hydro

#endif /* __h3o_InstanceMaker__ */
