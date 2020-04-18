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

#ifndef __h3o_ClassNode__
#define __h3o_ClassNode__

#include "BlockNode.hpp"
#include "PropertyNode.hpp"
#include "MethodNode.hpp"
#include "../../vm/Chunk.hpp"
#include "../../vm/ClassData.hpp"
#include "../../vm/Runtime.hpp"

namespace hydro
{

class ClassNode : public BlockNode
{
private:
	Chunk *_chunk;
	ClassData *_vclass;
	std::string _super;
	std::vector<H3oNode *> _fields;
    class ConstructorNode *_construct;
    
public:
    ClassNode(Chunk *chunk, ClassData *vclass, std::string superClassName = "");
	virtual ~ClassNode();
	virtual void build(Chunk *chunk) override;
	void addProperty(PropertyNode *prop);
	void addMethod(MethodNode *meth);
    void setConstructor(ConstructorNode *construct);
    ConstructorNode *getConstructor() const { return _construct; }
	ClassData *vclass() const { return _vclass; }
    std::string superClassName() const { return _super; }
};

} // namespace hydro

#endif /* __h3o_ClassNode__ */
