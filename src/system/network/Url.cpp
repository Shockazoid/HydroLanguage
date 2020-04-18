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

#include "Url.hpp"

namespace hydro
{

Url::Url(HvmEnv *env, HClass *urlClass) : HObject{env, urlClass} {}

Url::~Url() {}

void Url::construct(HvmEnv *env, std::string url)
{
    _url = url;
}

std::string Url::valueOf() const
{
    return _url;
}

} // namespace hydro
