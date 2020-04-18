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

#ifndef __h3o_HttpLoader__
#define __h3o_HttpLoader__

//#include <curl/curl.h>

#include "HttpRequest.hpp"

namespace hydro
{

class HttpLoader : public HObject
{
    friend HvmEnv;
    template <typename X>
    friend class NativeClassAllocator;
    
private:
    bool _loading;
    
    HttpLoader(HvmEnv *env, HClass *httpLoaderClass);
    void construct();
    
public:
    virtual ~HttpLoader();
    void load(object_ptr<HttpRequest> request);
    void unload();
    void cancel();
    bool isLoading() { return _loading; }
};

} // namespace hydro

#endif /* __h3o_HttpLoader__ */
