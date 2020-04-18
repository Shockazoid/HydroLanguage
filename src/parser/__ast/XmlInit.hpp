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

#ifndef __h3o_XmlInit__
#define __h3o_XmlInit__

#include <cstdlib>
#include "Literal.hpp"

namespace hydro
{

enum xml_init_type : uint16_t
{
	xml_init_doc,
	xml_init_element,
	xml_init_attr,
	xml_init_text,
	xml_init_comment,
	xml_init_doctype,
	xml_init_pi, // (?)
	xml_init_cdata
};

class XmlInit;

typedef XmlInit *ast_xml;

class XmlInit : public Literal
{
private:
	int _xmlNodeType;
	ast_xml _xmlParent;

public:
	XmlInit(lex_token token, int32_t xmlNodeType, ast_xml xmlParent = nullptr);
	virtual ~XmlInit();
	int32_t xmlNodeType() const { return _xmlNodeType; }
	ast_xml xmlParent() const { return _xmlParent; }
};

} // namespace hydro

#endif /* __h3o_XmlInit__ */
