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

#ifndef __h3o_FuncSymbol__
#define __h3o_FuncSymbol__

#include "TypeSymbol.hpp"

namespace hydro
{

class FuncSymbol : public Symbol
{
private:
	TypeSpec *_retType;
    PackageSymbol *_package;
    
public:
    FuncSymbol(Modifier *modifier, Name *name, Scope *ownScope, TypeSpec *returnType = nullptr, PackageSymbol *package = nullptr) : Symbol{modifier, name, ownScope, package}, _retType{returnType}, _package{package} {}
	virtual ~FuncSymbol() {}
    bool hasReturnType() const { return _retType != nullptr; }
    void returnType(TypeSpec *retType) { _retType = retType; }
	TypeSpec *returnType() const { return _retType; }
    PackageSymbol *package() const { return _package; }
};

} // namespace hydro

#endif /* __h3o_FuncSymbol__ */
