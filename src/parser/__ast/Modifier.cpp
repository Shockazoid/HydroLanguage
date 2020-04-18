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

#include "Modifier.hpp"

namespace hydro
{

Modifier::Modifier() {}

Modifier::~Modifier() {}

void Modifier::addMod(SimpleName *modifier)
{
	addChild(modifier);
}

bool Modifier::hasMod(std::string modifier)
{
	ast_node child = firstChild();

	while (child)
	{
        SimpleName *name = dynamic_cast<SimpleName *>(child);
        
		if (name->value() == modifier)
			return true; // exists
		child = child->next();
	}

	// fail
	return false;
}

SimpleName *Modifier::resolveMod(std::string modifier)
{
    ast_node child = firstChild();

    while (child)
    {
        SimpleName *name = dynamic_cast<SimpleName *>(child);
        
        if (name->value() == modifier)
            return name; // exists
        child = child->next();
    }

    
    // fail
    return nullptr;
}

uint16_t Modifier::mask() const
{
	uint16_t m = 0;
	ast_node child = firstChild();

	while (child)
	{
		m |= get_modifier(child->tokenValue());
		child = child->next();
	}

	return m;
}

} // namespace hydro
