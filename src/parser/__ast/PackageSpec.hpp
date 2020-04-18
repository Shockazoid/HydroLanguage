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

#ifndef __h3o_PackageSpec__
#define __h3o_PackageSpec__

#include "Ast.hpp"

namespace hydro
{

class PackageSpec : public Ast
{
public:
	PackageSpec(lex_token pkgToken);
	virtual ~PackageSpec();
	std::string package() const { return tokenValue().substr(1); }
};

typedef PackageSpec *ast_package;

} // namespace hydro

#endif /* __h3o_PackageSpec__ */
