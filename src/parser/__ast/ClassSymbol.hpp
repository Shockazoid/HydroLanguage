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

#ifndef __h3o_ClassSymbol__
#define __h3o_ClassSymbol__

#include <vector>

#include "TypeSpec.hpp"
#include "PackageSymbol.hpp"

namespace hydro
{

class ClassSymbol : public Symbol
{
private:
    PackageSymbol *_package;
	std::vector<TypeSpec *> _types;
    
public:
    ClassSymbol(Modifier *modifier, Name *name, Scope *ownScope, PackageSymbol *package) : Symbol{modifier, name, ownScope, package}, _types{}, _package{package} {}
	virtual ~ClassSymbol() {}
	void append(TypeSpec *type) { _types.push_back(type); }
	const std::vector<TypeSpec *> &types() const { return _types; }
    PackageSymbol *package() const { return _package; }
    std::string fullName() const { return _package ? _package->name()->value() + "::" + name()->value() : name()->value();
    }
};

} // namespace hydro

#endif /* __h3o_ClassSymbol__ */
