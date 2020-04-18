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

#include "HttpRequest.hpp"

namespace hydro
{

HttpRequest::HttpRequest(HvmEnv *env, HClass *httpRequestClass) : HObject{env, httpRequestClass} {}

HttpRequest::~HttpRequest() {}

void HttpRequest::construct(hurl url) { _url = url; }

} // namespace hydro
