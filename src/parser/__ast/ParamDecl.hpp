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

#ifndef __h3o_ParamDecl__
#define __h3o_ParamDecl__

#include "Decl.hpp"
#include "TypeSpec.hpp"
#include "Expr.hpp"
#include "ParamSymbol.hpp"

namespace hydro
{

class ParamDecl : public Decl
{
private:
    TypeSpec *_type;
    Expr *_defaultValue;
    
public:
    ParamDecl(Token *token, Modifier *mod, Name *name, TypeSpec *type, ParamSymbol *symbol, Expr *defaultValue = nullptr);
    virtual ~ParamDecl();
    TypeSpec *type() const { return _type; }
    Expr *defaultValue() const { return _defaultValue; }
};

} // namespace hydro

#endif /* __h3o_ParamDecl__ */
