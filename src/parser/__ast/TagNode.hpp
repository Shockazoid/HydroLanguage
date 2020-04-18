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

#ifndef __h3o_TagNode__
#define __h3o_TagNode__

#include "Ast.hpp"
#include "Name.hpp"

namespace hydro
{

class TagNode : public Ast
{
private:
    Name *_name;
public:
    TagNode(Name *name);
    virtual ~TagNode();
    Name *name() const { return _name; }
};

} // namespace hydro

#endif /* __h3o_TagNode__ */
