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

#ifndef __h3o_CastService__
#define __h3o_CastService__

#include <cstdlib>

#include "RuntimeService.hpp"
#include "../hvalue.hpp"
#include "../../system/HObject.hpp"
#include "../../system/HClass.hpp"
#include "../../system/HString.hpp"

namespace hydro
{

struct CastService : RuntimeService
{
    CastService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~CastService() {}
    hobject type(hclass type, hvalue subject) const { return nullptr; }
    std::string string(hvalue value) const;
    hstring hstring(hvalue value) const { return nullptr; }
    bool boolean(hvalue value) const { return false; }
    uint8_t uint8(hvalue value) const { return 0; }
    int8_t int8(hvalue value) const { return 0; }
    uint16_t uint16(hvalue value) const { return 0; }
    int16_t int16(hvalue value) const { return 0; }
    int32_t int32(hvalue value) const { return 0; }
    float float64(hvalue hvalue) const { return 0.0f; }
    double double64(hvalue hvalue) const { return 0.0; }
};

} // namespace hydro

#endif /* CastService_hpp */
