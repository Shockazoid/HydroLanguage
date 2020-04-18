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

#ifndef __h3o_ActExpr__
#define __h3o_ActExpr__

#include "Expr.hpp"
#include "Name.hpp"
#include "Block.hpp"

namespace hydro
{

class ActExpr : public Expr
{
private:
    Modifier *_mods;
    Name *_context;
    Block *_body;
    
public:
    ActExpr(Token *token, Modifier *mods, Name *context, Block *body);
    virtual ~ActExpr();
    Modifier *mods() const { return _mods; }
    Name *context() const { return _context; }
    Block *body() const { return _body; }
};

} // namespace hydro

#endif /* __h3o_ActExpr__ */
