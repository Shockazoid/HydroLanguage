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

#ifndef __h3o_Block__
#define __h3o_Block__

#include "Statement.hpp"

namespace hydro
{

class Block : public Statement
{
public:
    Block() : Statement{} {}
    virtual ~Block() {}
};

} // namespace hydro

#endif /* __h3o_Block__ */
