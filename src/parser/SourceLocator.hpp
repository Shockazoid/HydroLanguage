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

#ifndef __h3o_SourceLocator__
#define __h3o_SourceLocator__

#include "Source.hpp"

namespace hydro
{

class SourceLocator
{
public:
    SourceLocator() {}
    virtual ~SourceLocator() {}
    virtual Source *locate(std::string qualifiedName) const = 0;
};

} // namespace hydro

#endif /* __h3o_SourceLocator__ */
