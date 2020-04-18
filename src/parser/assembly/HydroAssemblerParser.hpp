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

#ifndef __h3o_HydroAssemblerParser__
#define __h3o_HydroAssemblerParser__

#include <stack>
#include <vector>

#include "HydroAssemblerLexer.hpp"
#include "nodes/BasicInstrNode.hpp"
#include "nodes/LabelNode.hpp"
#include "../../parser/BytecodeAssembler.hpp"
#include "../../system/HString.hpp"
#include "../../vm/opcodes.hpp"
#include "../../vm/hvalue.hpp"

namespace hydro
{

class HydroAssemblerParser : public BytecodeAssembler
{
private:
    std::stack<AssemblyNode *> _stack;
    std::vector<std::string> _names;
    std::vector<hvalue> _constPool;
    std::vector<LabelNode *> _labels;
    
    void push(AssemblyNode *node);
    void pop();
    void appendDefinition(AssemblyNode *def);
    void *getEnclosingFunc();
    void addSlot(AssemblyNode *node);
    void emit(AssemblyNode *node);
    void emit(hvm_isa_opcodes instr);
    void emit(hvm_isa_opcodes instr, int32_t opnd1);
    void emit(hvm_isa_opcodes instr, int32_t opnd1, int32_t opnd2);
    void emit(hvm_isa_opcodes instr, int32_t opnd1, int32_t opnd2, int32_t opnd3);
    uint32_t registerName(std::string name);
    uint32_t registerConstant(hvalue constant);
    LabelNode *defineLabel(std::string label);
    LabelNode *getLabel(std::string label);
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
    void parseInstr();
    
protected:
    virtual void parseNext() override;
    virtual void onParseInit() override;
    
public:
    HydroAssemblerParser(Compiler *compiler);
    virtual ~HydroAssemblerParser();
    
};

} // namespace hydro

#endif /* __h3o_HydroAssemblerParser__ */
