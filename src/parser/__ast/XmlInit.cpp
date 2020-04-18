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

#include "XmlInit.hpp"

namespace hydro
{

XmlInit::XmlInit(lex_token token, int32_t xmlNodeType, ast_xml xmlParent) : Literal{token}, _xmlNodeType{xmlNodeType}, _xmlParent{xmlParent}
{
	if (_xmlParent)
		_xmlParent->addChild(this);
}

XmlInit::~XmlInit() {}

} // namespace hydro
