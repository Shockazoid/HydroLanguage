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

#ifndef __h3o_ErrorService__
#define __h3o_ErrorService__

#include <memory>
#include <list>
#include <functional>

#include "RuntimeService.hpp"
#include "../AutoWireContext.hpp"
#include "../hvm_auto_wire.hpp"
#include "../hvm_ref_cast.hpp"
#include "../hvalue.hpp"
#include "../glue.hpp"
#include "../../system/HObject.hpp"
#include "../../system/HClass.hpp"
#include "../../system/HFunction.hpp"

namespace hydro
{

struct ErrorService : RuntimeService
{
private:
    
public:
    ErrorService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~ErrorService() {}
    bool pending() const;
    bool pending(HvmContext *cxt) const;
};

} // namespace hydro

#endif /* ErrorService_hpp */
