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

#ifndef Binding_hpp
#define Binding_hpp

#include <string>

#include "Modifier.hpp"

namespace hydro
{

class Binding
{
    friend Scope;
    friend Ast;

private:
    Modifier *_modifier;
    std::string _name;
    Ast *_owner;
    Symbol *_parent;
    class Scope *_ownScope;
    Scope *_enclosingScope;
    
public:
    Binding(Modifier *modifier, std::string name);
    virtual ~Binding();
    bool hasMod(std::string modifier) const;
    Modifier *modifier() const { return _modifier; }
    bool isAnonymous() const { return _name.empty(); }
    std::string name() const { return _name; }
    bool hasOwner() const { return _owner != nullptr; }
    Ast *owner() const { return _owner; }
    bool hasOwnScope() const { return _ownScope != nullptr; }
    Scope *ownScope() const { return _ownScope; }
    bool isDefined() const { return _enclosingScope != nullptr; }
    bool hasEnclosingScope() const { return isDefined(); } // alias
    Scope *enclosingScope() const { return _enclosingScope; }
    bool isBound() const { return _owner != nullptr; }
};

} // namespace hydro

#endif /* Binding_hpp */
