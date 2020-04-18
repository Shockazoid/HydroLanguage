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

#ifndef __h3o_H3oAssembler__
#define __h3o_H3oAssembler__

#include <stack>

#include "H3oAssemblerLexer.hpp"
#include "../parser/BytecodeAssembler.hpp"
#include "../parser/HydroCompiler.hpp"
#include "../vm/Chunk.hpp"
#include "../vm/FuncData.hpp"
#include "../vm/TypeSpecifier.hpp"
#include "../vm/NilTypeSpecifier.hpp"
#include "../vm/opcodes.hpp"
#include "nodes/InstrNode.hpp"
#include "nodes/Label.hpp"
#include "nodes/LabeledNode.hpp"
#include "nodes/NewNode.hpp"
#include "nodes/BlockNode.hpp"
#include "nodes/FuncNode.hpp"
#include "nodes/InstrGroupNode.hpp"
#include "nodes/ClassNode.hpp"
#include "nodes/ConstructorNode.hpp"
#include "nodes/PropertyNode.hpp"
#include "nodes/GetterNode.hpp"
#include "nodes/SetterNode.hpp"
#include "nodes/MethodNode.hpp"
#include "nodes/TryNode.hpp"
#include "nodes/EventNode.hpp"
#include "nodes/ActionNode.hpp"
#include "nodes/IntNode.hpp"
#include "../parser/errors.hpp"

namespace hydro
{

enum hydro_assembly_directive
{
	h3o_var_directive,
	h3o_func_directive,
	h3o_arg_directive,
	h3o_class_directive,
	h3o_extends_directive,
	h3o_implements_directive,
	h3o_construct_directive,
	h3o_property_directive,
	h3o_getter_directive,
	h3o_setter_directive,
	h3o_method_directive,
	h3o_action_directive,
	h3o_event_directive,
	h3o_etype_directive
};

class H3oAssembler : public BytecodeAssembler
{
private:
	Chunk *_result;
	FuncData *_mainFunc;
	FuncNode *_mainNode;
	std::vector<std::string> _names;
	std::vector<hvalue> _constPool;
	std::list<Label *> _labels;
	std::vector<TypeSpecifier *> _types;
	std::stack<BlockNode *> _stack;
	std::vector<BlockNode *> _blocks;
	MemorySpace *_scopeToUse;
	Runtime *_runtime;
	class HvmEnv *_env;

	void push(BlockNode *node);
	void pop();
	BlockNode *topNode() { return _stack.top(); }
	void addSlot(LocalVar *var);
	void addSlot(FuncData *func);
	void addSlot(ClassData *clss);
    FuncNode *getEnclosingFunc();
    EventNode *getContextParent();
    EventNode *getEventParent();
    EventNode *getEnclosingEvent();
    ClassNode *getEnclosingClass();
    ClassNode *getOwnerClass();
    bool isCurrentFunc(FuncNode *func) { return func == topNode(); }
    
	void validatePresence(hydro_assembly_directive dir);

	void emit(H3oNode *node);
    void emit(int32_t i);
	void emit(hvm_isa_opcodes instr);
	void emit(hvm_isa_opcodes instr, int32_t opnd1);
	void emit(hvm_isa_opcodes instr, int32_t opnd1, int32_t opnd2);
	void emit(hvm_isa_opcodes instr, int32_t opnd1, int32_t opnd2, int32_t opnd3);

	uint32_t registerName(std::string name);
	uint32_t registerConstant(hvalue constant);
	Label *defineLabel(std::string label);
	Label *getLabel(std::string label);

	void parseConst();
    void parseShortConst();
    void parseUshortConst();
	void parseIntConst();
	void parseUintConst();
    void parseLongConst();
    void parseUlongConst();
	void parseStringConst();
	void parseFloatConst();
	void parseDoubleConst();
	void parseCharConst();
	int32_t readInt();
	std::string readString();
	TypeSpecifier *parseTypeSpec();
	uint16_t parseMods();
	std::string readType();
	void parseFunc();
	void parseInstr();
	void parseBody(BlockNode *node, bool isDecl = true);
    void parseArg(FuncNode *node);
	InstrGroupNode *parseInstrGroup();
    void parseTryCatch();
	virtual void parseNext() override;
	void build();
    ClassNode *locateSuperClassNode(ClassNode *classNode);
    void buildClass(ClassNode *classNode, std::list<ClassNode *> &visited);

protected:
	virtual void onParseInit() override;
	virtual void onParseComplete() override;

public:
	H3oAssembler(HydroCompiler *compiler, Runtime *runtime);
	virtual ~H3oAssembler();
	Chunk *result() const { return _result; }
	Runtime *runtime() const { return _runtime; }
	MemorySpace *scope() const { return _scopeToUse; }
	HvmEnv *env() const { return _env; }
};

} // namespace hydro

#endif /* __h3o_H3oAssembler__ */
