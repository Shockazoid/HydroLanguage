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

#ifndef __h3o_SymbolTable__
#define __h3o_SymbolTable__

#include "Scope.hpp"
#include "BuiltinTypeSymbol.hpp"
#include "PackageSymbol.hpp"

namespace hydro
{

class SymbolTable : public Scope
{
private:
	void defineBuiltinTypes();

public:
	SymbolTable();
	virtual ~SymbolTable();
};

typedef SymbolTable *ast_globals;

} // namespace hydro

#endif /* __h3o_SymbolTable__ */
