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

#include "vpVisitor.hpp"

namespace hydro
{

vpElement::~vpElement() {}

void vpElement::accept(vp_visitor visitor)
{
	visitor->visit(this);
}

vpVisitor::~vpVisitor() {}

void vpVisitor::visit(vp_element element)
{
	// abstract
}

} // namespace hydro
