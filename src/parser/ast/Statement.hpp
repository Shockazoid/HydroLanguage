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

#ifndef __h3o_Statement__
#define __h3o_Statement__

#include "ASTNode.hpp"

namespace hydro
{

class Statement : public ASTNode
{
protected:
    Statement() {}
public:
    virtual ~Statement() {}
};

} // namespace hydro

#endif /* __h3o_Statement__ */
