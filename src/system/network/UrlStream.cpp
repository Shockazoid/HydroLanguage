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

#include "UrlStream.hpp"

namespace hydro
{

UrlStream::UrlStream(HvmEnv *env, HClass *urlStreamClass) : HObject{env, urlStreamClass} {}

UrlStream::~UrlStream() {}

void UrlStream::close()
{
    
}

} // namespace hydro
