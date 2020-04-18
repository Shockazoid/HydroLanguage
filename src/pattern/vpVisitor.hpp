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

#ifndef __h3o_vpVisitor__
#define __h3o_vpVisitor__

namespace hydro
{
typedef class vpElement *vp_element;

typedef class vpVisitor *vp_visitor;

class vpElement
{
public:
	virtual ~vpElement();
	virtual void accept(vp_visitor visitor);
};

class vpVisitor
{
public:
	virtual ~vpVisitor();
	virtual void visit(vp_element element);
};

} // namespace hydro

#endif /* __h3o_vpVisitor__ */
