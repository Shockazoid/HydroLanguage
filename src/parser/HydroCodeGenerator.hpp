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

#ifndef __h3o_HydroCodeGenerator__
#define __h3o_HydroCodeGenerator__

#include <stack>
#include <string>

#include "Compiler.hpp"
#include "hydro_compile_mode.hpp"
#include "HydroParser.hpp"
#include "errors.hpp"
#include "__ast/HydroVisitor.hpp"
#include "__ast/PropertySymbol.hpp"
#include "__ast/MethodSymbol.hpp"
#include "../vm/Runtime.hpp"
#include "../utility/classhelper.hpp"

namespace hydro
{

class HydroCodeGenerator : public HydroVisitor
{
protected:
	struct loop_data
	{
		std::string begin;
		std::string alt;
		std::string end;

		loop_data() : begin{}, alt{}, end{} {}
		loop_data(std::string b, std::string e) : begin{b}, alt{}, end{e} {}
		loop_data(std::string b, std::string a, std::string e) : begin{b}, alt{a}, end{e} {}
	};

	hydro_compile_mode _mode;
	std::string _h3oasm; // hydro assembly
	ast_program _subject;
	uint32_t _tab;
	uint64_t _nlabels;
	std::vector<loop_data *> _loops;
	Runtime *_runtime;
    Program *_program;
    uint32_t _line;
    std::string _source;
	static std::string unescapeTemplateString(lex_token stringToken)
	{
		std::string val = stringToken->value();
		const auto n = val.length();
		std::string s = "";

		for (int32_t i = 0; i < n; i++)
		{
			char ch = val[i];

			if (ch == '\\' && i + 1 < n && (val[i + 1] == '{' || val[i + 1] == '}'))
			{
				// unescape sequence
				ch = val[++i];
			}

			// append
			s += ch;
		}

		return s;
	}
    
    static VarSymbol *isVar(Symbol *symbol)
    {
        return dynamic_cast<VarSymbol *>(symbol);
    }
    
    static FuncSymbol *isFunc(Symbol *symbol)
    {
        return dynamic_cast<FuncSymbol *>(symbol);
    }
    
    static EventSymbol *isEvent(Symbol *symbol)
    {
        return dynamic_cast<EventSymbol *>(symbol);
    }
    
    static PropertySymbol *isProperty(Symbol *symbol)
    {
        return dynamic_cast<PropertySymbol *>(symbol);
    }
    
    static MethodSymbol *isMethod(Symbol *symbol)
    {
        return dynamic_cast<MethodSymbol *>(symbol);
    }
    
    static ClassSymbol *isClass(Symbol *symbol)
    {
        return dynamic_cast<ClassSymbol *>(symbol);
    }
    
    static std::string toAssemblyName(std::string name)
    {
        std::string assemblyName = "";
        bool fullname = false;
        
        for(int i = 0; i < name.length(); i++)
            if(name[i] == '.')
            {
                assemblyName += '/';
                fullname = true;
            }
            else
                assemblyName += name[i];
        
        return fullname ? '@' + assemblyName : assemblyName;
    }
    
    static std::string funcToAssemblyName(PackageSymbol *pkg, FuncSymbol *func)
    {
        std::string assemblyName = toAssemblyName(pkg->fullPackageName()) + '/' + func->name()->value();
        if(assemblyName[0] != '@')
            assemblyName = '@' + assemblyName; // prepend expected @
        return assemblyName;
    }
    
    static std::string eventToAssemblyName(PackageSymbol *pkg, EventSymbol *evnt)
    {
        std::string assemblyName = toAssemblyName(pkg->fullPackageName()) + '/' + evnt->name()->value();
        if(assemblyName[0] != '@')
            assemblyName = '@' + assemblyName; // prepend expected @
        return assemblyName;
    }
    
    static std::string classToAssemblyName(PackageSymbol *pkg, ClassSymbol *clss)
    {
        std::string assemblyName = toAssemblyName(pkg->fullPackageName()) + '/' + clss->name()->value();
        if(assemblyName[0] != '@')
            assemblyName = '@' + assemblyName; // prepend expected @
        return assemblyName;
    }
    
	bool evalMode() const { return _mode == h3o_compile_eval; }

	void tabUp();
	void tabDown();
	void emit(std::string instr);
	void emit(std::string instr, std::string opnd1);
	void emit(std::string instr, std::string opnd1, std::string opnd2);
	void emit(std::string instr, std::string opnd1, std::string opnd2, std::string opnd3);
	void comment(std::string content);
	std::string loadClass(SimpleName *id);
	Symbol *resolveLocal(std::string name);
	Symbol *resolveUpvalue(std::string name);
	//int32_t addUpvalue(int32_t index, bool local);
	void tryResolveTypeBindings(ClassDecl *clss);
	ClassSymbol *tryResolveSuperClass(ClassDecl *clss);
	std::string createModString(Modifier *mods);
	std::string createModString(uint16_t mods);
	std::string toAssemblyType(TypeSpec *type);
	std::string nameQualifierToAssemblyName(BinaryOp *nq);
	std::string packageToAssemblyName(PackageSymbol *pkg);
	void assign(BinaryOp *op);
	void chain(BinaryOp *op);
	std::string newLabel() { return "Label" + std::to_string(_nlabels++); }
	void pushLoop(std::string start, std::string alt, std::string done);
	void popLoop();
    
    Symbol *getEnclosingSymbol();
    Symbol *getEnclosingSymbol(Scope *scope);
    FuncSymbol *getEnclosingFunc();
    FuncSymbol *getEnclosingFunc(Scope *scope);
    ClassSymbol *getParentClass();
    ClassSymbol *getParentClass(Scope *scope);
    ClassSymbol *getEnclosingClass();
    ClassSymbol *getEnclosingClass(Scope *scope);
    ConstructorSymbol *getEnclosingConstructor();
    ConstructorSymbol *getEnclosingConstructor(Scope *scope);
    PropertySymbol *getEnclosingProperty();
    PropertySymbol *getEnclosingProperty(Scope *scope);
    MethodSymbol *getEnclosingMethod();
    MethodSymbol *getEnclosingMethod(Scope *scope);
    int32_t addSlot();
    
    int32_t getLocalIndex(Symbol *symbol, Scope *scopeToUse);
    
	loop_data *startLoop();
	loop_data *startLoop(bool alt);
	loop_data *loopCond();
	loop_data *loopAlt();
	loop_data *endLoop();
	loop_data *jumpLoop();
	loop_data *loopBack(int k);
	loop_data *breakLoop(int k, BreakStmt *brk);
	loop_data *continueLoop(int k, ContinueStmt *cont);

	Call *findSuperCall(Stmt *body);
	bool parentDeclIsClass(Ast *child);
    void emitNamedCall(std::string opcode, Call *call, NamedCallArgs *args);

	// common nodes
	virtual void visitProgram(Program *prog) override;
	virtual void visitStmt(Stmt *stmt) override;
	//virtual void visitBlock(Block *block) override;
	//virtual void visitExpr(Expr *expr) override;

	// expressions
	//virtual void visitEmpty(EmptyExpr *expr) override;
	//virtual void visitNested(Nested *expr) override;
	//virtual void visitLiteral(Literal *expr) override;
	virtual void visitIdentifier(SimpleName *id) override;
	virtual void visitNull(Literal *expr) override;
	virtual void visitUndefined(Literal *expr) override;
	virtual void visitPackage(PackageDecl *pkg) override;
	virtual void visitIdentity(Literal *expr) override;
	virtual void visitBool(Literal *expr) override;
	virtual void visitChar(Literal *expr) override;
	virtual void visitString(Literal *expr) override;
	virtual void visitStringTemplate(Literal *tpl) override;
	virtual void visitUint(Literal *expr) override;
	virtual void visitInt(Literal *expr) override;
	virtual void visitHexadecimal(Literal *expr) override;
	virtual void visitBinary(Literal *expr) override;
	virtual void visitOctal(Literal *expr) override;
	virtual void visitUshort(Literal *expr) override;
	virtual void visitShort(Literal *expr) override;
	virtual void visitUlong(Literal *expr) override;
	virtual void visitLong(Literal *expr) override;
	virtual void visitFloat(Literal *expr) override;
	virtual void visitDouble(Literal *expr) override;
	virtual void visitInfinity(Literal *expr) override;
	virtual void visitNaN(Literal *expr) override;
	virtual void visitRegex(Literal *expr) override;
	virtual void visitObject(ObjectInit *expr) override;
	virtual void visitObjectProp(ObjectInitProperty *expr) override;
	virtual void visitJson(JsonInit *expr) override;
	virtual void visitJsonProp(JsonInitProperty *expr) override;
	virtual void visitJsonValue(Expr *expr) override;
	virtual void visitArray(ArrayInit *expr) override;
	virtual void visitArrayElement(Expr *expr) override;
	virtual void visitDict(DictInit *expr) override;
	virtual void visitDictEntry(DictInitEntry *expr) override;
	virtual void visitXml(XmlInit *expr) override;
	virtual void visitLambda(AFunc *lambda) override; // anonymous functions

	// operators
	virtual void visitUnaryOp(UnaryOp *op) override;
	virtual void visitBinaryOp(BinaryOp *op) override;
	virtual void visitAssign(BinaryOp *op) override;
	virtual void visitTernaryOp(TernaryOp *op) override;
	virtual void visitCall(Call *call) override;
	virtual void visitCallArgs(CallArgs *args) override;

	// statements
	virtual void visitEmptyStmt(EmptyStmt *stmt) override;
	//virtual void visitExprStmt(ExprStmt *stmt) override;
	//virtual void visitBody(Stmt *body) override;
	virtual void visitIf(IfStmt *stmt) override;
	virtual void visitThrow(ThrowStmt *stmt) override;
	virtual void visitTry(TryStmt *stmt) override;
    virtual void visitCatch(CatchClause *stmt) override;
	virtual void visitFor(ForStmt *stmt) override;
	virtual void visitForEach(ForEachStmt *stmt) override;
	virtual void visitDo(DoStmt *stmt) override;
	virtual void visitWhile(WhileStmt *stmt) override;
	virtual void visitContinue(ContinueStmt *stmt) override;
	virtual void visitBreak(BreakStmt *stmt) override;
	virtual void visitSwitch(SwitchStmt *stmt) override;
	virtual void visitCase(CaseClause *stmt) override;
	virtual void visitDefault(DefaultClause *stmt) override;
	virtual void visitReturn(ReturnStmt *stmt) override;
	//virtual void visitImport(ImportStmt *stmt) override;
    
	// declarations
	virtual void visitName(Name *name) override;
	virtual void visitPackageSpec(PackageSpec *pkg) override;
	//virtual void visitDecl(Decl* decl) override;
	virtual void visitVar(VarDecl *var) override;
	//virtual void visitConst(ConstDecl* cnst) override;
	//virtual void visitLet(LetDecl* let) override;
	void emitFuncBody(FuncDecl *func, bool emitBlock = true);
	virtual void visitFunc(FuncDecl *func) override;
	virtual void visitClass(ClassDecl *clss) override;
	virtual void visitConstructor(ConstructorDecl *construct) override;
	virtual void visitProperty(PropertyDecl *prop) override;
	virtual void visitGetter(GetterDecl *get) override;
	virtual void visitSetter(SetterDecl *set) override;
	virtual void visitMethod(MethodDecl *meth) override;
	virtual void visitAction(ActDecl *act) override;
	virtual void visitEvent(EventDecl *evnt) override;
	virtual void visitEtype(EtypeDecl *etype) override;
    virtual void visitParam(ParamDecl *param) override;
	virtual void visitInterface(InterfaceDecl *it) override;
	virtual void visitEnum(EnumDecl *enm) override;
	//virtual void visitElement(ElementDecl *elem) override;
	//virtual void visitState(StateDecl *state) override;
    PackageSymbol *loadEnclosingPackage();
    
public:
	HydroCodeGenerator(class HydroCompiler *compiler, Scope *globals, Runtime *runtime);
	virtual ~HydroCodeGenerator();
	void generate(Program *program, Scope *scopeToUse, hydro_compile_mode mode);
    virtual void visit(vp_element e) override;
	hydro_compile_mode mode() const { return _mode; }
	std::string assembly() const { return _h3oasm; }
	Runtime *runtime() const { return _runtime; }
};

} // namespace hydro

#endif /* __h3o_HydroCodeGenerator__ */
