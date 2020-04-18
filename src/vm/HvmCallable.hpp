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

#ifndef HvmCallable_hpp
#define HvmCallable_hpp

#include <list>
#include <string>
#include <map>

#include "hvalue.hpp"

namespace hydro
{

class HvmCallable
{
public:
    virtual hvalue call(HvmContext *threadContext, VM *vm, std::list<hvalue> &args, hvalue thisObject = nullptr) = 0;
    virtual hvalue call(HvmContext *threadContext, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject = nullptr) = 0;
};

} // namespace hydro

#endif /* HvmCallable_hpp */
