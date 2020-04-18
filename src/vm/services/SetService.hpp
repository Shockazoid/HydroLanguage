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

#ifndef __h3o_SetService__
#define __h3o_SetService__

#include <string>
#include <functional>

#include "RuntimeService.hpp"
#include "../hvalue.hpp"
#include "../VM_Func.hpp"
#include "../glue.hpp"
#include "../../system/HObject.hpp"
#include "../../system/HFunction.hpp"

namespace hydro
{

struct SetService : public RuntimeService
{
private:
    void globalFn(std::string name, function_glue *glue) const;
    
public:
    SetService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~SetService() {}
    void global(std::string name, hvalue value) const;
    template <typename ReturnType, typename... Arg>
    void global(std::string name, ReturnType (*fn)(Arg...)) const
    {
        // create glue
        function_glue *glue = new native_function{fn};
        globalFn(name, glue);
    }
    void property(hobject instance, std::string name, hvalue value) const;
    void index(hobject instance, hvalue key, hvalue value) const;
};

} // namespace hydro

#endif /* __h3o_SetService__ */
