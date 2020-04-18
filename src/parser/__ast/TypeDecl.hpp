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

#ifndef __h3o_TypeDecl__
#define __h3o_TypeDecl__

#include "ScopedDecl.hpp"

namespace hydro
{

class TypeDecl : public ScopedDecl
{
private:
    std::list<Name *> _types;

public:
    TypeDecl(lex_token token, ast_mod mods, ast_name name, ast_stmt body, PackageSymbol *package = nullptr, Symbol *symbol = nullptr);
    virtual ~TypeDecl();
    void appendType(Name *name) { _types.push_back(name); }
    const std::list<Name *> &types() const { return _types; }
};

} // namespace hydro

#endif /* __h3o_TypeDecl__ */
