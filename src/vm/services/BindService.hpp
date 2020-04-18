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

#ifndef __h3o_BindService__
#define __h3o_BindService__

#include "RuntimeService.hpp"
#include "../Closure.hpp"
#include "../../system/HClass.hpp"
#include "../../system/HFunction.hpp"
#include "../../system/HAction.hpp"
#include "../../system/HEventContext.hpp"
#include "../../system/HConstructor.hpp"
#include "../../system/HMethod.hpp"
#include "../../system/HProperty.hpp"
#include "../../system/HGetter.hpp"
#include "../../system/HSetter.hpp"

namespace hydro
{

struct BindService : RuntimeService
{
private:
    Closure *createClosure(const VMObject *data) const;

public:
    BindService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~BindService() {}
    hvalue ifNotExists(Runtime *runtime, const ClassData *vclass) const;
    hvalue ifNotExists(Runtime *runtime, const FuncData *vfunc) const;
    hvalue ifNotExists(Runtime *runtime, const ActionData *vact) const;
    hvalue ifNotExists(Runtime *runtime, const EventData *vevent) const;
};

} // namespace hydro

#endif /* __h3o_BindService__ */
