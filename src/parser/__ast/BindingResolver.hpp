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

#ifndef __h3o_BindingResolver__
#define __h3o_BindingResolver__

#include "HydroVisitor.hpp"

namespace hydro
{

class BindingResolver : protected HydroVisitor
{
protected:
    class HydroParser *_parser;
    
public:
    BindingResolver(class HydroCompiler *compiler, HydroParser *parser, Scope *globals);
    virtual ~BindingResolver();
    Symbol *tryResolve(Name *name, Scope *scopeToUse = nullptr);
    Symbol *tryResolve(Expr *expr, Scope *scopeToUse = nullptr);
};

} // namespace hydro

#endif /* __h3o_BindingResolver__ */
