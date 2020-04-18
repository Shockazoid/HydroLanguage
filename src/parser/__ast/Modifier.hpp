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

#ifndef __h3o_Modifiers__
#define __h3o_Modifiers__

#include "Ast.hpp"
#include "SimpleName.hpp"
#include "../../vm/modifiers.hpp"

namespace hydro
{

class Modifier : public Ast
{
public:
	Modifier();
	virtual ~Modifier();
	void addMod(SimpleName *modifier);
	bool hasMod(std::string modifier);
    SimpleName *resolveMod(std::string modifier);
	uint16_t mask() const;
};

typedef Modifier *ast_mod;

} // namespace hydro

#endif /* __h3o_Modifiers__ */
