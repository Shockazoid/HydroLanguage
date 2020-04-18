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

#include "BindingResolver.hpp"
#include "../HydroParser.hpp"

namespace hydro
{

BindingResolver::BindingResolver(HydroCompiler *compiler, HydroParser *parser, Scope *globals) : HydroVisitor(compiler, globals), _parser{parser} {}

BindingResolver::~BindingResolver() {}

Symbol *BindingResolver::tryResolve(Name *name, Scope *scopeToUse)
{
    if(SimpleName *simple = dynamic_cast<SimpleName *>(name))
    {
        // try resolve simple name
        
    }
    else if(QualifiedName *qualifiedName = dynamic_cast<QualifiedName *>(name))
    {
        // try resolve qualified name
        
    }
    
    // fail
    return nullptr;
}

Symbol *BindingResolver::tryResolve(Expr *expr, Scope *scopeToUse)
{
    if(!scopeToUse)
        scopeToUse = _globals;
    
    if(BinaryOp *bin = dynamic_cast<BinaryOp *>(expr))
    {
        std::string op = bin->tokenValue();
        Expr *lhs = bin->lhs();
        Expr *rhs = bin->rhs();
        Symbol *symbol;
        
        if(op == "::")
        {
            // try resolve name qualifier (scope resolution)
        }
        else if(op == ".")
        {
            // try resolve property
            symbol = tryResolve(lhs, scopeToUse);
            
            if(symbol && symbol->ownScope())
            {
                if(SimpleName *id = dynamic_cast<SimpleName *>(rhs))
                {
                    // try resolve member
                    tryResolve(id, symbol->ownScope());
                }
            }
        }
        else if(op == "->")
        {
            // try resolve chain
        }
    }
    // fail
    return nullptr;
}

} // namespace hydro
