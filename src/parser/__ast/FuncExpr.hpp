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

#ifndef __h3o_FuncExpr__
#define __h3o_FuncExpr__

#include "Expr.hpp"
#include "FuncDecl.hpp"
#include "Block.hpp"

namespace hydro
{

class FuncExpr : public Expr
{
private:
    Modifier *_mods;
    FuncArgs *_args;
    Block *_body;
    
public:
    FuncExpr(Token *token, Modifier *mods, FuncArgs *args, TypeSpec *retType, Block *body);
    virtual ~FuncExpr();
    Modifier *mods() const { return _mods; }
    FuncArgs *args() const { return _args; }
    Block *body() const { return _body; }
};

} // namespace hydro

#endif /* __h3o_FuncExpr__ */
