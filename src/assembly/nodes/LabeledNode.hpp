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

#ifndef __h3o_LabeledNode__
#define __h3o_LabeledNode__

#include "Label.hpp"

namespace hydro
{

class LabeledNode : public H3oNode
{
private:
	Label *_label;

public:
	LabeledNode(Label *label);
	virtual ~LabeledNode();
	virtual void build(Chunk *chunk) override;
};

} // namespace hydro

#endif /* __h3o_LabeledNode__ */
