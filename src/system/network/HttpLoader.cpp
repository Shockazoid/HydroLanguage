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

#include "HttpLoader.hpp"

namespace hydro
{

HttpLoader::HttpLoader(HvmEnv *env, HClass *httpLoaderClass) : HObject{env, httpLoaderClass}, _loading{false} {}

HttpLoader::~HttpLoader() {}

void HttpLoader::construct() {}

void HttpLoader::load(object_ptr<HttpRequest> request)
{
    
}

void HttpLoader::unload()
{
    
}

void HttpLoader::cancel()
{
    
}

} // namespace hydro
